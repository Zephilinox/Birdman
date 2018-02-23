#include "NetworkingState.hpp"

//SELF
#include "../Architecture/GameData.hpp"
#include "../States/MenuState.hpp"
#include "../States/FadeOutState.hpp"

NetworkingState::NetworkingState(GameData* game_data)
	: BaseState(game_data)
	, netman(game_data->getNetworkManager())
	, menu(game_data)
{	
	menu.addButton(game_data->getWindowWidth() / 2 - 80, game_data->getWindowHeight() / 2 - 40, "SERVER", ASGE::COLOURS::DIMGRAY, ASGE::COLOURS::ANTIQUEWHITE);
	menu.addButton(game_data->getWindowWidth() / 2 - 80, game_data->getWindowHeight() / 2 + 40, "CLIENT", ASGE::COLOURS::DIMGRAY, ASGE::COLOURS::ANTIQUEWHITE);
	menu.addButton(game_data->getWindowWidth() / 2 - 80, game_data->getWindowHeight() / 2 + 120, "BACK", ASGE::COLOURS::DIMGRAY, ASGE::COLOURS::ANTIQUEWHITE);

	menu.getButton(0).on_click.connect([&]()
	{
		netman->initialize(true);
		netman->client_connected.connect(this, &NetworkingState::onClientConnected);
		netman->client_disconnected.connect(this, &NetworkingState::onClientDisconnected);
		netman->client_sent_packet.connect(this, &NetworkingState::onClientSentPacket);
	});

	menu.getButton(1).on_click.connect([&]()
	{
		netman->initialize(false);
		netman->connected.connect(this, &NetworkingState::onConnected);
		netman->disconnected.connect(this, &NetworkingState::onDisconnected);
		netman->server_sent_packet.connect(this, &NetworkingState::onServerSentPacket);
	});

	menu.getButton(2).on_click.connect([game_data]()
	{
		game_data->getStateManager()->pop();
	});

	entities.emplace_back(std::make_unique<Paddle>(game_data));
	entities.emplace_back(std::make_unique<Paddle>(game_data));
	entities.emplace_back(std::make_unique<Ball>(game_data));
	serverPaddle = static_cast<Paddle*>(entities[0].get());
	clientPaddle = static_cast<Paddle*>(entities[1].get());
	serverBall = static_cast<Ball*>(entities[2].get());

	serverPaddle->entity_info.networkID = 0;
	serverPaddle->entity_info.ownerID = 0;
	serverPaddle->sprite.addFrame("Portraits/blabbering_npc", 1);
	serverPaddle->sprite.xPos = 60;
	serverPaddle->sprite.yPos = 720 / 2;

	clientPaddle->entity_info.networkID = 1;
	clientPaddle->entity_info.ownerID = 1;
	clientPaddle->sprite.addFrame("Portraits/blabbering_npc", 1);
	clientPaddle->sprite.xPos = 1100;
	clientPaddle->sprite.yPos = 720 / 2;

	serverBall->entity_info.networkID = 2;
	serverBall->entity_info.ownerID = 0;
	serverBall->sprite.addFrame("UI/DialogueMarker", 1);
	serverBall->sprite.xPos = 1280 / 2;
	serverBall->sprite.yPos = 720 / 2;
}

NetworkingState::~NetworkingState()
{
	netman->reset();
}

void NetworkingState::update(const ASGE::GameTime& gt)
{
	if (netman->isInitialized())
	{
		float dt = gt.delta_time.count() / 1000.0f;

		for (auto& ent : entities)
		{
			ent->update(dt);
		}

		if (netman->isServer())
		{
			updateServer(dt);
		}
		else if (netman->isConnected())
		{
			updateClient(dt);
		}
	}
	else
	{
		menu.update();
	}
}

void NetworkingState::render() const
{
	if (netman->isInitialized())
	{
		for (const auto& ent : entities)
		{
			ent->render(game_data->getRenderer());
		}
	}
	else
	{
		menu.render();
	}
}

void NetworkingState::onActive()
{
}

void NetworkingState::onInactive()
{
}

void NetworkingState::updateServer(float dt)
{
	//Simple AABB collision check
	if (serverPaddle->sprite.xPos < serverBall->sprite.xPos + serverBall->sprite.getCurrentFrameSprite()->width() &&
		serverPaddle->sprite.xPos + serverPaddle->sprite.getCurrentFrameSprite()->width() > serverBall->sprite.xPos &&
		serverPaddle->sprite.yPos < serverBall->sprite.yPos + serverBall->sprite.getCurrentFrameSprite()->height() &&
		serverPaddle->sprite.yPos + serverPaddle->sprite.getCurrentFrameSprite()->height() > serverBall->sprite.yPos)
	{
		serverBall->movingLeft = false;
		serverBall->dirY = game_data->getRandomNumberGenerator()->getRandomFloat(-0.8, 0.8);
	}

	if (clientPaddle->sprite.xPos < serverBall->sprite.xPos + serverBall->sprite.getCurrentFrameSprite()->width() &&
		clientPaddle->sprite.xPos + clientPaddle->sprite.getCurrentFrameSprite()->width() > serverBall->sprite.xPos &&
		clientPaddle->sprite.yPos < serverBall->sprite.yPos + serverBall->sprite.getCurrentFrameSprite()->height() &&
		clientPaddle->sprite.yPos + clientPaddle->sprite.getCurrentFrameSprite()->height() > serverBall->sprite.yPos)
	{
		serverBall->movingLeft = true;
		serverBall->dirY = game_data->getRandomNumberGenerator()->getRandomFloat(-0.8, 0.8);
	}
	
	Packet p;
	p.setID(hash("Entity"));
	p << &serverPaddle->entity_info
		<< serverPaddle->sprite.xPos
		<< serverPaddle->sprite.yPos;
	netman->sendPacket(0, &p);

	p.reset();
	p.setID(hash("Entity"));
	p << &serverBall->entity_info
		<< serverBall->sprite.xPos
		<< serverBall->sprite.yPos;
	netman->sendPacket(0, &p);
}

void NetworkingState::updateClient(float dt)
{
	Packet p;
	p.setID(hash("Entity"));
	p << &clientPaddle->entity_info
		<< clientPaddle->sprite.xPos
		<< clientPaddle->sprite.yPos;
	netman->sendPacket(0, &p);
}

void NetworkingState::onClientConnected(ClientInfo* ci)
{
	std::cout << "client " << ci->id << " connected \n";
	clientPaddle->entity_info.ownerID = ci->id;
}

void NetworkingState::onClientDisconnected(uint32_t client_id)
{
}

void NetworkingState::onClientSentPacket(const enet_uint8 channel_id, ClientInfo* ci, Packet p)
{
	switch (p.getID())
	{
		case hash("Entity"):
		{
			EntityInfo info;
			p >> &info;
			Entity* ent = getEntity(info.networkID);
			if (ent && //exists
				ent->entity_info.ownerID == info.ownerID && //owners match
				info.ownerID == ci->id && //client isn't lying about what it owns
				ent->entity_info.type == info.type) //types match
			{
				switch (info.type)
				{
					case hash("Paddle"):
					{
						Paddle* paddle = static_cast<Paddle*>(ent);
						p >> paddle->sprite.xPos >> paddle->sprite.yPos;
					} break;
					case hash("Ball"):
					{
						Ball* ball = static_cast<Ball*>(ent);
						p >> ball->sprite.xPos >> ball->sprite.yPos;
					} break;
				}
			}
		} break;
	}
}

void NetworkingState::onConnected()
{
}

void NetworkingState::onDisconnected()
{
}

void NetworkingState::onServerSentPacket(const enet_uint8 channel_id, Packet p)
{
	switch (p.getID())
	{
		case hash("Entity"):
		{
			EntityInfo info;
			p >> &info;
			Entity* ent = getEntity(info.networkID);
			if (ent && //exists
				ent->entity_info.ownerID == info.ownerID && //owners match
				ent->entity_info.type == info.type) //types match
			{
				switch (info.type)
				{
					case hash("Paddle"):
					{
						Paddle* paddle = static_cast<Paddle*>(ent);
						p >> paddle->sprite.xPos >> paddle->sprite.yPos;
					} break;
					case hash("Ball"):
					{
						Ball* ball = static_cast<Ball*>(ent);
						p >> ball->sprite.xPos >> ball->sprite.yPos;
					} break;
				}
			}
		} break;
	}
}

Entity* NetworkingState::getEntity(uint32_t networkID)
{
	auto& it = std::find_if(entities.begin(), entities.end(), [networkID](const auto& ent)
	{
		return ent->entity_info.networkID == networkID;
	});

	if (it != entities.end())
	{
		return it->get();
	}

	return nullptr;
}
