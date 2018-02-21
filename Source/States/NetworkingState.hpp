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

//https://stackoverflow.com/questions/17471717/c-static-cast-from-int-to-void-to-char-can-you-help-me-to-understand-this
//http://www.cplusplus.com/forum/general/109866/
//https://stackoverflow.com/questions/2437283/c-c-packing-signed-char-into-int
//https://stackoverflow.com/questions/1874354/a-dynamic-buffer-type-in-c
//https://stackoverflow.com/questions/16543519/serialization-of-struct

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
		deserializePosition = sizeof(HashedID);
		buffer.resize(sizeof(HashedID));
		buffer.reserve(255);
	}

	void setID(HashedID id)
	{
		memcpy(buffer.data(), &id, sizeof(id));
	}

	HashedID getID()
	{
		HashedID id;
		memcpy(&id, buffer.data(), sizeof(id));
		return id;
	}

	void serialize(void* data, size_t size)
	{
		//grab the old size
		auto old_size = buffer.size();
		//resize the vector so we know we have enough space to append more bits
		buffer.resize(old_size + size);
		//append the data, at the position before in the vector before it was resized
		memcpy(buffer.data() + old_size, data, size);
	}

	void serialize(std::string src)
	{
		serialize(static_cast<int32_t>(src.size()));
		serialize(src.data(), src.size());
	}

	void serialize(int32_t src)
	{
		serialize(&src, sizeof(src));
	}

	void serialize(float src)
	{
		serialize(&src, sizeof(src));
	}

	void serialize(bool src)
	{
		serialize(&src, sizeof(src));
	}

	void deserialize(void* destination, size_t size)
	{
		memcpy(destination, buffer.data() + deserializePosition, size);
		deserializePosition += size;
	}

	void deserialize(int32_t& destination)
	{
		deserialize(&destination, sizeof(destination));
	}

	void deserialize(std::string& destination)
	{
		int32_t size;
		deserialize(size);
		destination.resize(size);
		deserialize(destination.data(), size);
	}

	void deserialize(float& destination)
	{
		deserialize(&destination, sizeof(destination));
	}

	void deserialize(bool& destination)
	{
		deserialize(&destination, sizeof(destination));
	}

	std::vector<enet_uint8> buffer;
	size_t deserializePosition = 0;
};

struct Entity
{
	int32_t id;
	std::string name;
	float x;
	float y;
	bool alive;

	void serialize(Packet& p)
	{
		p.serialize(id);
		p.serialize(name);
		p.serialize(x);
		p.serialize(y);
		p.serialize(alive);
	}

	void deserialize(Packet& p)
	{
		p.deserialize(id);
		p.deserialize(name);
		p.deserialize(x);
		p.deserialize(y);
		p.deserialize(alive);
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
