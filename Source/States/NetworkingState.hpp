#pragma once

//STD
#include <chrono>

//SELF
#include "../Architecture/States/BaseState.hpp"
#include "../Architecture/Timer.hpp"
#include "../Architecture/Messages/Message.hpp"
#include "../Architecture/Managers/NetworkManager.hpp"
#include "../Architecture/AnimatedSprite.hpp"
#include "../Architecture/UI/Menu.hpp"

class GameData;

class Entity;
class Paddle;
class Ball;

class NetworkingState : public BaseState
{
public:
	NetworkingState(GameData* game_data);
	~NetworkingState();

	void update(const ASGE::GameTime&) override final;
	void render() const override final;
	void onActive() override final;
	void onInactive() override final;

	template <class T, class... Args>
	void createEntity(Args... args)
	{
		assert(netman->isInitialized());

		if (netman->isServer())
		{
			entities.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
			entities.back()->entity_info.networkID = next_network_id;
			next_network_id++;
			entities.back()->entity_info.ownerID = netman->clientID;

			//Send packets to all clients (except us, of course)
			Packet p;
			p.setID(hash("CreateEntity"));
			p << &entities.back()->entity_info;
			netman->sendPacket(0, &p);
		}
		else
		{
			//Send packet to server with what we want to create
			T ent(game_data);
			Packet p;
			p.setID(hash("CreateEntity"));
			p << &ent.entity_info;
			netman->sendPacket(0, &p);
		}
	}

	template <class T, class... Args>
	void createEntity(uint32_t ownerID, Args... args)
	{
		assert(netman->isInitialized() && netman->isServer());

		entities.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
		entities.back()->entity_info.networkID = next_network_id;
		next_network_id++;
		entities.back()->entity_info.ownerID = ownerID;

		//Send packet to all clients
		Packet p;
		p.setID(hash("CreateEntity"));
		p << &entities.back()->entity_info;
		netman->sendPacket(0, &p);
	}

private:
	//Server
	void updateServer(float dt);
	void onClientConnected(ClientInfo* ci) noexcept;
	void onClientDisconnected(uint32_t client_id) noexcept;
	void onClientSentPacket(const enet_uint8 channel_id, ClientInfo* ci, Packet p);

	//Client
	void updateClient(float dt);
	void onConnected() noexcept;
	void onDisconnected() noexcept;
	void onServerSentPacket(const enet_uint8 channel_id, Packet p);

	//Both
	void onPacketReceived(const enet_uint8 channel_id, ClientInfo* ci, Packet p);
	Entity* getEntity(uint32_t networkID);

	NetworkManager* netman;
	Menu menu;
	std::vector<std::unique_ptr<Entity>> entities;

	//Server
	Paddle* serverPaddle;
	Ball* serverBall;
	unsigned int next_network_id = 0;

	//Client
	Paddle* clientPaddle;
};
