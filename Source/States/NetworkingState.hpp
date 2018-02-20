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

struct Packet
{
	Packet()
	{
		buffer.reserve(255);
	}

	void serialize(std::string string)
	{
		//grab the old size
		auto old_size = buffer.size();
		//resize the vector so we know we have enough space to append more bits
		buffer.resize(old_size + string.size());
		//copy the string data to the end of the buffer at the position before it was resized
		memcpy(buffer.data() + old_size, string.data(), string.size());
	}

	void serialize(int i)
	{
		//grab the old size
		auto old_size = buffer.size();
		//resize the vector so we know we have enough space to append more bits
		buffer.resize(old_size + sizeof(i));
		//grab the pointer to i, and the size of it, so it will grab the data between those using pointer arithmetic. smiley face.
		memcpy(buffer.data() + old_size, &i, sizeof(i));
	}

	HashedID id;
	std::vector<enet_uint8> buffer;
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
