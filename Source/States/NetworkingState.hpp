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
#include "../Architecture/Networking/Packet.hpp"

class GameData;

struct ServerClient
{
	unsigned int id;
	unsigned int get_id() const
	{
		return id;
	}
};

struct Entity
{
	int32_t id;
	std::string name;
	float x;
	float y;
	bool alive;

	//Needed because we have private data
	friend Packet& operator<<(Packet& p, const Entity& entity);
	friend Packet& operator>>(Packet& p, Entity& entity);

private:
	int stuffThatNeedsSerialized;
};

//https://stackoverflow.com/questions/32232448/already-defined-error-with-operator-overloading
inline
Packet& operator <<(Packet& packet, const Entity& entity)
{
	return packet
		<< entity.id
		<< entity.name
		<< entity.x
		<< entity.y
		<< entity.alive
		<< entity.stuffThatNeedsSerialized;
}

inline
Packet& operator >>(Packet& packet, Entity& entity)
{
	return packet
		>> entity.id
		>> entity.name
		>> entity.x
		>> entity.y
		>> entity.alive
		>> entity.stuffThatNeedsSerialized;
}

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
