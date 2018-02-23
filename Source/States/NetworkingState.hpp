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

struct EntityInfo
{
	uint32_t networkID;
	uint32_t ownerID;
	HashedID type;
};

inline
Packet& operator <<(Packet& p, EntityInfo* e)
{
	p << e->networkID
		<< e->ownerID
		<< e->type;
	return p;
}

inline
Packet& operator >>(Packet& p, EntityInfo* e)
{
	p >> e->networkID
		>> e->ownerID
		>> e->type;
	return p;
}

class Entity
{
public:
	virtual void update(float dt) = 0;
	virtual void render(ASGE::Renderer* renderer) const = 0;

	EntityInfo entity_info;
};

inline
Packet& operator <<(Packet& p, Entity* e)
{
	p << &e->entity_info;
	return p;
}

inline
Packet& operator >>(Packet& p, Entity* e)
{
	p >> &e->entity_info;
	return p;
}

class Paddle : public Entity
{
public:
	Paddle(GameData* game_data)
		: sprite(game_data->getRenderer())
		, game_data(game_data)
	{
		entity_info.type = hash("Paddle");
	}

	void update(float dt) override final
	{
		sprite.update(dt);

		if (entity_info.ownerID == game_data->getNetworkManager()->clientID)
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

class Ball : public Entity
{
public:
	Ball(GameData* game_data)
		: sprite(game_data->getRenderer())
		, game_data(game_data)
	{
		entity_info.type = hash("Ball");
	}

	void update(float dt) override final
	{
		sprite.update(dt);
		
		if (game_data->getNetworkManager()->isServer())
		{
			if (sprite.yPos < 0)
			{
				dirY = -dirY;
				sprite.yPos = 0;
			}
			
			if (sprite.yPos + sprite.getCurrentFrameSprite()->height() > game_data->getWindowHeight())
			{
				dirY = -dirY;
				sprite.yPos = game_data->getWindowHeight() - sprite.getCurrentFrameSprite()->height();
			}

			if (sprite.xPos + sprite.getCurrentFrameSprite()->width() < 0 ||
				sprite.xPos > game_data->getWindowWidth())
			{
				sprite.xPos = 1280 / 2;
				dirY = 0;
				movingLeft = game_data->getRandomNumberGenerator()->getRandomInt(0, 1);
			}

			sprite.xPos += 200 * dt * (movingLeft ? -1 : 1);
			sprite.yPos += 200 * dt * dirY;
		}
	}

	void render(ASGE::Renderer* renderer) const override final
	{
		renderer->renderSprite(*sprite.getCurrentFrameSprite());
	}

	AnimatedSprite sprite;
	GameData* game_data;
	bool movingLeft = false;
	float dirY;
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
	Entity* getEntity(uint32_t networkID);

	NetworkManager* netman;
	Menu menu;

	//Server
	Paddle* serverPaddle;
	Ball* serverBall;

	//Client
	Paddle* clientPaddle;

	std::vector<std::unique_ptr<Entity>> entities;
};
