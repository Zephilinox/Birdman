#pragma once

//STD
#include <chrono>

//LIB
#include <enetpp\server.h>
#include <enetpp\client.h>

//SELF
#include "../Architecture/States/BaseState.hpp"
#include "../Architecture/Timer.hpp"
#include "../Architecture/Messages/Message.hpp"
#include "../Architecture/Managers/NetworkManager.hpp"
#include "../Architecture/AnimatedSprite.hpp"
#include "../Architecture/UI/Menu.hpp"

class GameData;

class NetworkingState : public BaseState
{
public:
	NetworkingState(GameData* game_data);
	~NetworkingState();

	void update(const ASGE::GameTime&) override final;
	void render() const override final;
	void onActive() override final;
	void onInactive() override final;
	void updateServer(float dt);
	void updateClient(float dt);

	//Server
	void onClientConnected(ClientInfo* ci);
	void onClientDisconnected(uint32_t client_id);
	void onClientSentPacket(ClientInfo* ci, Packet p);

	//Client
	void onConnected();
	void onDisconnected();
	void onServerSentPacket(Packet p);

private:
	NetworkManager* netman;
	Menu menu;

	//Server
	AnimatedSprite serverPaddle;
	AnimatedSprite serverBall;
	bool ballMovingLeft = true;
	float ballDirY;

	//Client
	AnimatedSprite clientPaddle;
};
