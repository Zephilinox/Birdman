#include "NetworkingState.hpp"

//SELF
#include "../Architecture/GameData.hpp"
#include "../States/MenuState.hpp"
#include "../States/FadeOutState.hpp"

NetworkingState::NetworkingState(GameData* game_data)
	: BaseState(game_data)
	, netman(game_data->getNetworkManager())
	, serverPaddleMaster(game_data->getRenderer())
	, serverPaddleSlave(game_data->getRenderer())
{
	int input;
	std::cout << "server is 0, client is 1: ";
	std::cin >> input;
	std::cout << "\n";
	
	netman->initialize(!input);
	
	//not really necessary but meh
	if (netman->isServer())
	{
		netman->client_connected.connect(this, &NetworkingState::onClientConnected);
		netman->client_disconnected.connect(this, &NetworkingState::onClientDisconnected);
		netman->client_sent_packet.connect(this, &NetworkingState::onClientSentPacket);
	}
	else
	{
		netman->connected.connect(this, &NetworkingState::onConnected);
		netman->disconnected.connect(this, &NetworkingState::onDisconnected);
		netman->server_sent_packet.connect(this, &NetworkingState::onServerSentPacket);
	}

	serverPaddleMaster.addFrame("Portraits/blabbering_npc", 1);
	serverPaddleMaster.xPos = 60;
	serverPaddleMaster.yPos = 720 / 2;

	serverPaddleSlave.addFrame("Portraits/blabbering_npc", 1);
	serverPaddleSlave.xPos = 60;
	serverPaddleSlave.yPos = 720 / 2;
}

NetworkingState::~NetworkingState()
{
	netman->reset();
}

void NetworkingState::update(const ASGE::GameTime& gt)
{
	if (netman->isServer())
	{
		updateServer(gt.delta_time.count() / 1000.0f);
	}
	else if (netman->isConnected())
	{
		updateClient(gt.delta_time.count() / 1000.0f);
	}
}

void NetworkingState::render() const
{
	if (netman->isServer())
	{
		game_data->getRenderer()->renderSprite(*serverPaddleMaster.getCurrentFrameSprite());
	}
	else
	{
		game_data->getRenderer()->renderSprite(*serverPaddleSlave.getCurrentFrameSprite());
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
	serverPaddleMaster.update(dt);

	Entity ent;
	ent.id = game_data->getRandomNumberGenerator()->getRandomInt(0, 10000);
	ent.name = "Big Baddy Dude " + std::to_string(game_data->getRandomNumberGenerator()->getRandomInt(0, 99));
	ent.x = game_data->getRandomNumberGenerator()->getRandomFloat(-100, 1000);
	ent.y = game_data->getRandomNumberGenerator()->getRandomFloat(-1000, 100);
	ent.alive = game_data->getRandomNumberGenerator()->getRandomInt(0, 1);

	Packet updateEntity;
	updateEntity << ent;
	updateEntity.setID(hash("UpdateEntity"));

	std::string data = "Hi";
	Packet msg;
	msg << data;

	if (t.getElapsedTime() > 1)
	{
		t.restart();
		netman->sendPacket(0, &msg);
		netman->sendPacket(0, &updateEntity);
	}

	if (game_data->getInputManager()->isKeyDown(ASGE::KEYS::KEY_W))
	{
		serverPaddleMaster.yPos -= 1000 * dt;
	}
	else if (game_data->getInputManager()->isKeyDown(ASGE::KEYS::KEY_S))
	{
		serverPaddleMaster.yPos += 1000 * dt;
	}

	Packet p;
	p.setID(hash("UpdatePosition"));
	p << serverPaddleMaster.xPos << serverPaddleMaster.yPos;
	netman->sendPacket(0, &p);
}

void NetworkingState::updateClient(float dt)
{
	serverPaddleSlave.update(dt);

	if (t.getElapsedTime() > 1)
	{
		t.restart();
		Packet p;
		p << std::to_string(t2.getElapsedTime());
		netman->sendPacket(0, &p);
	}
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
	std::cout << "client " << ci->id << ": ";
	std::string str;
	p >> str;
	std::cout << str << "\n";
}

void NetworkingState::onConnected()
{
}

void NetworkingState::onDisconnected()
{
}

void NetworkingState::onServerSentPacket(Packet p)
{
	if (p.getID() == hash("UpdateEntity"))
	{
		Entity entReceived;
		p >> entReceived;
		std::cout << "Received UpdateEntity packet for entity "
			<< entReceived.id
			<< " (" << entReceived.name << ") "
			<< " at position "
			<< entReceived.x << ", " << entReceived.y
			<< " life status = " << entReceived.alive << "\n";
	}
	else if (p.getID() == hash("UpdatePosition"))
	{
		p >> serverPaddleSlave.xPos >> serverPaddleSlave.yPos;
	}
	else
	{
		std::string msg;
		p >> msg;
		std::cout << "Message Received: " << msg << " from packet type " << p.getID() << "\n";
	}
}
