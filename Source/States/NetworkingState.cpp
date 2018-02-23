#include "NetworkingState.hpp"

//SELF
#include "../Architecture/GameData.hpp"
#include "../States/MenuState.hpp"
#include "../States/FadeOutState.hpp"

NetworkingState::NetworkingState(GameData* game_data)
	: BaseState(game_data)
	, netman(game_data->getNetworkManager())
	, serverBall(game_data->getRenderer())
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
	serverPaddle = static_cast<Paddle*>(entities[0].get());
	clientPaddle = static_cast<Paddle*>(entities[1].get());

	serverPaddle->networkID = 0;
	serverPaddle->ownerID = 0;
	serverPaddle->sprite.addFrame("Portraits/blabbering_npc", 1);
	serverPaddle->sprite.xPos = 60;
	serverPaddle->sprite.yPos = 720 / 2;

	clientPaddle->networkID = 1;
	clientPaddle->ownerID = 1;
	clientPaddle->sprite.addFrame("Portraits/blabbering_npc", 1);
	clientPaddle->sprite.xPos = 1100;
	clientPaddle->sprite.yPos = 720 / 2;

	serverBall.addFrame("UI/DialogueMarker", 1);
	serverBall.xPos = 1280 / 2;
	serverBall.yPos = 720 / 2;
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

		serverBall.update(dt);

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
		if (netman->isServer())
		{
			serverPaddle->render(game_data->getRenderer());
			if (!netman->server.get_connected_clients().empty())
			{
				clientPaddle->render(game_data->getRenderer());
			}
		}
		else
		{
			clientPaddle->render(game_data->getRenderer());
			if (netman->isConnected())
			{
				serverPaddle->render(game_data->getRenderer());
			}
		}

		game_data->getRenderer()->renderSprite(*serverBall.getCurrentFrameSprite());
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
	if (serverPaddle->sprite.xPos < serverBall.xPos + serverBall.getCurrentFrameSprite()->width() &&
		serverPaddle->sprite.xPos + serverPaddle->sprite.getCurrentFrameSprite()->width() > serverBall.xPos &&
		serverPaddle->sprite.yPos < serverBall.yPos + serverBall.getCurrentFrameSprite()->height() &&
		serverPaddle->sprite.yPos + serverPaddle->sprite.getCurrentFrameSprite()->height() > serverBall.yPos)
	{
		ballMovingLeft = false;
		ballDirY = game_data->getRandomNumberGenerator()->getRandomFloat(-0.8, 0.8);
	}

	if (clientPaddle->sprite.xPos < serverBall.xPos + serverBall.getCurrentFrameSprite()->width() &&
		clientPaddle->sprite.xPos + clientPaddle->sprite.getCurrentFrameSprite()->width() > serverBall.xPos &&
		clientPaddle->sprite.yPos < serverBall.yPos + serverBall.getCurrentFrameSprite()->height() &&
		clientPaddle->sprite.yPos + clientPaddle->sprite.getCurrentFrameSprite()->height() > serverBall.yPos)
	{
		ballMovingLeft = true;
		ballDirY = game_data->getRandomNumberGenerator()->getRandomFloat(-0.8, 0.8);
	}

	if (serverBall.yPos < 0 ||
		serverBall.yPos + serverBall.getCurrentFrameSprite()->width() > game_data->getWindowHeight())
	{
		ballDirY = -ballDirY;
	}

	if (serverBall.yPos + serverBall.getCurrentFrameSprite()->width() < 0 ||
		serverBall.yPos > game_data->getWindowWidth())
	{
		serverBall.xPos = 1280 / 2;
	}

	serverBall.xPos += 200 * dt * (ballMovingLeft ? -1 : 1);
	serverBall.yPos += 200 * dt * ballDirY;

	Packet p;
	p.setID(hash("Entity"));
	p << serverPaddle->networkID
		<< serverPaddle->ownerID
		<< serverPaddle->type
		<< serverPaddle->sprite.xPos
		<< serverPaddle->sprite.yPos;
	netman->sendPacket(0, &p);

	p.reset();
	p.setID(hash("UpdateBall"));
	p << serverBall.xPos << serverBall.yPos;
	netman->sendPacket(0, &p);
	netman->sendPacket(1, &p);
}

void NetworkingState::updateClient(float dt)
{
	Packet p;
	p.setID(hash("Entity"));
	p << clientPaddle->networkID
		<< clientPaddle->ownerID
		<< clientPaddle->type
		<< clientPaddle->sprite.xPos
		<< clientPaddle->sprite.yPos;
	netman->sendPacket(0, &p);
	netman->sendPacket(1, &p);
}

void NetworkingState::onClientConnected(ClientInfo* ci)
{
	std::cout << "client " << ci->id << " connected \n";
}

void NetworkingState::onClientDisconnected(uint32_t client_id)
{
}

void NetworkingState::onClientSentPacket(const enet_uint8 channel_id, ClientInfo* ci, Packet p)
{
	if (p.getID() == hash("Entity"))
	{
		uint32_t networkID;
		uint32_t ownerID;
		HashedID type;
		p >> networkID >> ownerID >> type;

		if (entities[networkID]->ownerID == ownerID &&
			ownerID == ci->id)
		{
			if (type == hash("Paddle"))
			{
				Paddle* paddle = static_cast<Paddle*>(entities[networkID].get());
				p >> paddle->sprite.xPos >> paddle->sprite.yPos;
			}
		}
		else
		{
			assert(true);
		}
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
	if (p.getID() == hash("Entity"))
	{
		uint32_t networkID;
		uint32_t ownerID;
		HashedID type;
		p >> networkID >> ownerID >> type;

		//todo client doesn't care who owns what?
		if (entities[networkID]->ownerID == ownerID)
		{
			if (type == hash("Paddle"))
			{
				Paddle* paddle = static_cast<Paddle*>(entities[networkID].get());
				p >> paddle->sprite.xPos >> paddle->sprite.yPos;
			}
		}
	}
	else if (p.getID() == hash("UpdateBall"))
	{
		p >> serverBall.xPos >> serverBall.yPos;
	}
}
