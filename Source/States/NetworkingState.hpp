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

	//Client
	AnimatedSprite clientPaddle;
};
