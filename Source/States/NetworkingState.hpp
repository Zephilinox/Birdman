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

	//Client
	Paddle* clientPaddle;
};
