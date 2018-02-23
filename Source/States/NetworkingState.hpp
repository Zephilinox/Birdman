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

class Entity
{
public:
	virtual void update(float dt) = 0;
	virtual void render(ASGE::Renderer* renderer) const = 0;

	uint32_t networkID;
	uint32_t ownerID;
	HashedID type;
};

class Paddle : public Entity
{
public:
	Paddle(GameData* game_data)
		: sprite(game_data->getRenderer())
		, game_data(game_data)
	{
		type = hash("Paddle");
	}

	void update(float dt) override final
	{
		sprite.update(dt);

		if (ownerID == game_data->getNetworkManager()->clientID)
		{
			if (game_data->getInputManager()->isKeyDown(ASGE::KEYS::KEY_W))
			{
				sprite.yPos -= 1000 * dt;
			}
			else if (game_data->getInputManager()->isKeyDown(ASGE::KEYS::KEY_S))
			{
				sprite.yPos += 1000 * dt;
			}
		}
	}

	void render(ASGE::Renderer* renderer) const override final
	{
		renderer->renderSprite(*sprite.getCurrentFrameSprite());
	}

	AnimatedSprite sprite;
	GameData* game_data;
};

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
	void onClientSentPacket(const enet_uint8 channel_id, ClientInfo* ci, Packet p);

	//Client
	void onConnected();
	void onDisconnected();
	void onServerSentPacket(const enet_uint8 channel_id, Packet p);

private:
	NetworkManager* netman;
	Menu menu;

	//Server
	Paddle* serverPaddle;
	AnimatedSprite serverBall;

	//Client
	Paddle* clientPaddle;

	//both
	bool ballMovingLeft = true;
	float ballDirY;

	std::vector<std::unique_ptr<Entity>> entities;
};
