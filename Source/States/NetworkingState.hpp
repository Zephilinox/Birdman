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
class GameData;

struct ServerClient
{
	unsigned int id;
	unsigned int get_id() const
	{
		return id;
	}
};

/**
*  See BaseState for overriden functions
*/
class NetworkingState : public BaseState
{
public:
	NetworkingState(GameData* game_data);
	~NetworkingState();

	void update(const ASGE::GameTime&) override final;
	void render() const override final;
	void onActive() override final;
	void onInactive() override final;
	void updateServer();
	void updateClient();

private:
	bool isServer = true;

	//Server
	enetpp::server<ServerClient> server;
	unsigned int next_uid = 0;
	Timer t;

	//Client
	enetpp::client client;
	Timer t2;
};
