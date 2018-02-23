#include "NetworkingState.hpp"

//SELF
#include "../Architecture/GameData.hpp"
#include "../States/MenuState.hpp"
#include "../States/FadeOutState.hpp"

NetworkingState::NetworkingState(GameData* game_data)
	: BaseState(game_data)
	, netman(game_data->getNetworkManager())
	, serverPaddle(game_data->getRenderer())
	, clientPaddle(game_data->getRenderer())
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

	serverPaddle.addFrame("Portraits/blabbering_npc", 1);
	serverPaddle.xPos = 60;
	serverPaddle.yPos = 720 / 2;

	clientPaddle.addFrame("Portraits/blabbering_npc", 1);
	clientPaddle.xPos = 1100;
	clientPaddle.yPos = 720 / 2;

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
		serverPaddle.update(dt);
		clientPaddle.update(dt);
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
		game_data->getRenderer()->renderSprite(*serverPaddle.getCurrentFrameSprite());
		game_data->getRenderer()->renderSprite(*clientPaddle.getCurrentFrameSprite());
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
	if (serverPaddle.xPos < serverBall.xPos + serverBall.getCurrentFrameSprite()->width() &&
		serverPaddle.xPos + serverPaddle.getCurrentFrameSprite()->width() > serverBall.xPos &&
		serverPaddle.yPos < serverBall.yPos + serverBall.getCurrentFrameSprite()->height() &&
		serverPaddle.yPos + serverPaddle.getCurrentFrameSprite()->height() > serverBall.yPos)
	{
		ballMovingLeft = false;
		ballDirY = game_data->getRandomNumberGenerator()->getRandomFloat(-0.8, 0.8);
	}

	if (clientPaddle.xPos < serverBall.xPos + serverBall.getCurrentFrameSprite()->width() &&
		clientPaddle.xPos + clientPaddle.getCurrentFrameSprite()->width() > serverBall.xPos &&
		clientPaddle.yPos < serverBall.yPos + serverBall.getCurrentFrameSprite()->height() &&
		clientPaddle.yPos + clientPaddle.getCurrentFrameSprite()->height() > serverBall.yPos)
	{
		ballMovingLeft = true;
		ballDirY = game_data->getRandomNumberGenerator()->getRandomFloat(-0.8, 0.8);
	}

	if (serverBall.yPos < 0 ||
		serverBall.yPos + serverBall.getCurrentFrameSprite()->width() > game_data->getWindowHeight())
	{
		ballDirY = -ballDirY;
	}

	if (game_data->getInputManager()->isKeyDown(ASGE::KEYS::KEY_W))
	{
		serverPaddle.yPos -= 1000 * dt;
	}
	else if (game_data->getInputManager()->isKeyDown(ASGE::KEYS::KEY_S))
	{
		serverPaddle.yPos += 1000 * dt;
	}

	if (serverBall.yPos + serverBall.getCurrentFrameSprite()->width() < 0 ||
		serverBall.yPos > game_data->getWindowWidth())
	{
		serverBall.xPos = 1280 / 2;
	}

	serverBall.xPos += 200 * dt * (ballMovingLeft ? -1 : 1);
	serverBall.yPos += 200 * dt * ballDirY;

	Packet p;
	p.setID(hash("UpdatePosition"));
	p << 0 << serverPaddle.xPos << serverPaddle.yPos;
	netman->sendPacket(0, &p);

	p.reset();
	p.setID(hash("UpdatePosition"));
	p << 2 << serverBall.xPos << serverBall.yPos;
	netman->sendPacket(0, &p);
}

void NetworkingState::updateClient(float dt)
{
	if (game_data->getInputManager()->isKeyDown(ASGE::KEYS::KEY_W))
	{
		clientPaddle.yPos -= 1000 * dt;
	}
	else if (game_data->getInputManager()->isKeyDown(ASGE::KEYS::KEY_S))
	{
		clientPaddle.yPos += 1000 * dt;
	}

	Packet p;
	p.setID(hash("UpdatePosition"));
	p << 1 << clientPaddle.xPos << clientPaddle.yPos;
	netman->sendPacket(0, &p);
}

void NetworkingState::onClientConnected(ClientInfo* ci)
{
	std::cout << "client " << ci->id << " connected \n";
}

void NetworkingState::onClientDisconnected(uint32_t client_id)
{
}

void NetworkingState::onClientSentPacket(ClientInfo* ci, Packet p)
{
	if (p.getID() == hash("UpdatePosition"))
	{
		int objectID;
		p >> objectID;

		switch (objectID)
		{
			//serverPaddle
			case 0:
				break;
			//clientPaddle
			case 1:
				p >> clientPaddle.xPos >> clientPaddle.yPos;
				break;
			//serverBall
			case 2:
				break;
		}
	}
}

void NetworkingState::onConnected()
{
}

void NetworkingState::onDisconnected()
{
}

void NetworkingState::onServerSentPacket(Packet p)
{
	if (p.getID() == hash("UpdatePosition"))
	{
		int objectID;
		p >> objectID;

		switch (objectID)
		{
			//serverPaddle
			case 0:
				p >> serverPaddle.xPos >> serverPaddle.yPos;
				break;
			//clientPaddle
			case 1:
				break;
			//serverBall
			case 2:
				p >> serverBall.xPos >> serverBall.yPos;
				break;
		}
	}
}
