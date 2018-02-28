#include "NetworkingState.hpp"

//SELF
#include "../Architecture/GameData.hpp"
#include "../States/MenuState.hpp"
#include "../States/FadeOutState.hpp"

struct EntityInfo
{
	uint32_t networkID;
	uint32_t ownerID;
	HashedID type;
};

Packet& operator <<(Packet& p, EntityInfo* e)
{
	p << e->networkID
		<< e->ownerID
		<< e->type;
	return p;
}

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
	virtual ~Entity() = default;
	virtual void update(float dt) = 0;
	virtual void render(ASGE::Renderer* renderer) const = 0;
	virtual void receivedPacket(uint32_t channelID, Packet* p) = 0;

	EntityInfo entity_info;
};

Packet& operator <<(Packet& p, Entity* e)
{
	p << &e->entity_info;
	return p;
}

Packet& operator >>(Packet& p, Entity* e)
{
	p >> &e->entity_info;
	return p;
}

class Paddle : public Entity
{
public:
	Paddle()
		: sprite(GameData::getRenderer())
	{
		entity_info.type = hash("Paddle");
		sprite.addFrame("Portraits/blabbering_npc", 1);

		if (GameData::getNetwork()->isServer())
		{
			sprite.xPos = 60;
			sprite.yPos = 720 / 2;
		}
		else
		{
			sprite.xPos = 1100;
			sprite.yPos = 720 / 2;
		}
	}

	void update(float dt) override final
	{
		sprite.update(dt);

		if (entity_info.ownerID == GameData::getNetwork()->clientID)
		{
			if (GameData::getInput()->isKeyDown(ASGE::KEYS::KEY_W))
			{
				sprite.yPos -= 1000 * dt;
			}
			else if (GameData::getInput()->isKeyDown(ASGE::KEYS::KEY_S))
			{
				sprite.yPos += 1000 * dt;
			}

			Packet p;
			p.setID(hash("Entity"));
			p << &entity_info
				<< sprite.xPos
				<< sprite.yPos;
			GameData::getNetwork()->sendPacket(0, &p);
		}
	}

	void render(ASGE::Renderer* renderer) const override final
	{
		renderer->renderSprite(*sprite.getCurrentFrameSprite());
	}

	void receivedPacket(uint32_t channel_id, Packet* p) override final
	{
		*p >> sprite.xPos >> sprite.yPos;
	}

	AnimatedSprite sprite;
};

class Ball : public Entity
{
public:
	Ball()
		: sprite(GameData::getRenderer())
	{
		entity_info.type = hash("Ball");
		sprite.addFrame("UI/DialogueMarker", 1);
		sprite.xPos = 1280 / 2;
		sprite.yPos = 720 / 2;
	}

	void update(float dt) override final
	{
		sprite.update(dt);

		if (GameData::getNetwork()->isServer())
		{
			if (sprite.yPos < 0)
			{
				dirY = -dirY;
				sprite.yPos = 0;
			}

			if (sprite.yPos + sprite.getCurrentFrameSprite()->height() > GameData::getWindowHeight())
			{
				dirY = -dirY;
				sprite.yPos = GameData::getWindowHeight() - sprite.getCurrentFrameSprite()->height();
			}

			if (sprite.xPos + sprite.getCurrentFrameSprite()->width() < 0 ||
				sprite.xPos > GameData::getWindowWidth())
			{
				sprite.xPos = 1280 / 2;
				dirY = 0;
				movingLeft = GameData::getRNG()->getRandomInt(0, 1);
			}

			sprite.xPos += 200 * dt * (movingLeft ? -1 : 1);
			sprite.yPos += 200 * dt * dirY;
		}

		if (entity_info.ownerID == GameData::getNetwork()->clientID)
		{
			Packet p;
			p.setID(hash("Entity"));
			p << &entity_info
				<< sprite.xPos
				<< sprite.yPos;
			GameData::getNetwork()->sendPacket(0, &p);
		}
	}

	void render(ASGE::Renderer* renderer) const override final
	{
		renderer->renderSprite(*sprite.getCurrentFrameSprite());
	}

	void receivedPacket(uint32_t channelID, Packet* p) override final
	{
		*p >> sprite.xPos >> sprite.yPos;
	}

	AnimatedSprite sprite;
	bool movingLeft = false;
	float dirY = 0;
};

NetworkingState::NetworkingState()
	: BaseState(false)
	, netman(GameData::getNetwork())
{	
	menu.addButton(GameData::getWindowWidth() / 2 - 80, GameData::getWindowHeight() / 2 - 40, "SERVER", ASGE::COLOURS::DIMGRAY, ASGE::COLOURS::ANTIQUEWHITE);
	menu.addButton(GameData::getWindowWidth() / 2 - 80, GameData::getWindowHeight() / 2 + 40, "CLIENT", ASGE::COLOURS::DIMGRAY, ASGE::COLOURS::ANTIQUEWHITE);
	menu.addButton(GameData::getWindowWidth() / 2 - 80, GameData::getWindowHeight() / 2 + 120, "BACK", ASGE::COLOURS::DIMGRAY, ASGE::COLOURS::ANTIQUEWHITE);

	menu.getButton(0).on_click.connect([this]()
	{
		GameData::getNetwork()->initialize(true);
		managedOnConnect = GameData::getNetwork()->client_connected.connect(this, &NetworkingState::onClientConnected);
		managedOnDisconnect = GameData::getNetwork()->client_disconnected.connect(this, &NetworkingState::onClientDisconnected);
		managedOnSentPacket = GameData::getNetwork()->client_sent_packet.connect(this, &NetworkingState::onClientSentPacket);

		createEntity<Paddle>();
		createEntity<Ball>();
		serverPaddle = static_cast<Paddle*>(getEntity(1));
		serverBall = static_cast<Ball*>(getEntity(2));
	});

	menu.getButton(1).on_click.connect([this]()
	{
		GameData::getNetwork()->initialize(false);
		managedOnConnect = GameData::getNetwork()->connected.connect(this, &NetworkingState::onConnected);
		managedOnDisconnect = GameData::getNetwork()->disconnected.connect(this, &NetworkingState::onDisconnected);
		managedOnSentPacket = GameData::getNetwork()->server_sent_packet.connect(this, &NetworkingState::onServerSentPacket);
	});

	menu.getButton(2).on_click.connect([]()
	{
		GameData::getStates()->pop();
	});
}

NetworkingState::~NetworkingState()
{
	netman->reset();
}

void NetworkingState::update(const ASGE::GameTime& gt)
{
	if (netman->isInitialized())
	{
		const float dt = gt.delta_time.count() / 1000.0f;

		for (auto& ent : entities)
		{
			ent->update(dt);
		}

		if (netman->isServer())
		{
			updateServer(dt);
		}
		else if (netman->isConnected())
		{
			updateClient(dt);
		}
	}
	else
	{
		menu.update();
	}
}

void NetworkingState::render() const
{
	if (netman->isInitialized())
	{
		for (const auto& ent : entities)
		{
			ent->render(GameData::getRenderer());
		}
	}
	else
	{
		menu.render();
	}
}

void NetworkingState::onActive()
{
}

void NetworkingState::onInactive()
{
}

void NetworkingState::updateServer(float dt)
{
	//Simple AABB collision check
	if (serverPaddle->sprite.xPos < serverBall->sprite.xPos + serverBall->sprite.getCurrentFrameSprite()->width() &&
		serverPaddle->sprite.xPos + serverPaddle->sprite.getCurrentFrameSprite()->width() > serverBall->sprite.xPos &&
		serverPaddle->sprite.yPos < serverBall->sprite.yPos + serverBall->sprite.getCurrentFrameSprite()->height() &&
		serverPaddle->sprite.yPos + serverPaddle->sprite.getCurrentFrameSprite()->height() > serverBall->sprite.yPos)
	{
		serverBall->movingLeft = false;
		serverBall->dirY = GameData::getRNG()->getRandomFloat(-0.8, 0.8);
	}

	uint32_t clientPaddleID = 0;
	for (const auto& ent : entities)
	{
		if (ent->entity_info.ownerID != 1 &&
			ent->entity_info.type == hash("Paddle")) //1 is server id
		{
			clientPaddleID = ent->entity_info.networkID;
			break;
		}
	}
	clientPaddle = static_cast<Paddle*>(getEntity(clientPaddleID));
	if (clientPaddle)
	{
		if (clientPaddle->sprite.xPos < serverBall->sprite.xPos + serverBall->sprite.getCurrentFrameSprite()->width() &&
			clientPaddle->sprite.xPos + clientPaddle->sprite.getCurrentFrameSprite()->width() > serverBall->sprite.xPos &&
			clientPaddle->sprite.yPos < serverBall->sprite.yPos + serverBall->sprite.getCurrentFrameSprite()->height() &&
			clientPaddle->sprite.yPos + clientPaddle->sprite.getCurrentFrameSprite()->height() > serverBall->sprite.yPos)
		{
			serverBall->movingLeft = true;
			serverBall->dirY = GameData::getRNG()->getRandomFloat(-0.8, 0.8);
		}
	}
}

void NetworkingState::updateClient(float dt)
{
}

void NetworkingState::onClientConnected(ClientInfo* ci) noexcept
{
	//Send all existing entity information to the client

	Packet p;
	for (const auto& ent : entities)
	{
		p.reset();
		p.setID(hash("CreateEntity"));
		p << &ent->entity_info;
		netman->sendPacket(ci->id, 0, &p);
	}
}

void NetworkingState::onClientDisconnected(uint32_t client_id) noexcept
{
	std::experimental::erase_if(entities, [client_id](const auto& entity)
	{
		return entity->entity_info.ownerID == client_id;
	});
}

void NetworkingState::onClientSentPacket(const enet_uint8 channel_id, ClientInfo* ci, Packet p)
{
	onPacketReceived(channel_id, ci, std::move(p));
}

void NetworkingState::onConnected() noexcept
{
}

void NetworkingState::onDisconnected() noexcept
{
}

void NetworkingState::onServerSentPacket(const enet_uint8 channel_id, Packet p)
{
	onPacketReceived(channel_id, nullptr, std::move(p));
}

void NetworkingState::onPacketReceived(const enet_uint8 channel_id, ClientInfo* ci, Packet p)
{
	switch (p.getID())
	{
		case hash("Entity"):
		{
			EntityInfo info;
			p >> &info;
			Entity* ent = getEntity(info.networkID);
			if (ent && //exists
				ent->entity_info.ownerID == info.ownerID && //owners match
				ent->entity_info.type == info.type) //types match
			{
				if (!ci || //client received packet, we trust the server
					(ci && info.ownerID == ci->id)) //received packet from client, make sure they aren't lying about what they own
				{
					ent->receivedPacket(channel_id, &p);
				}
			}
		} break;
		case hash("CreateEntity"):
		{
			//so if client sends a spawn ent, just trust it. create the ent with the owner id and entity id it gave us. but then the network ID might not be right, so.. we send back the network id?
			EntityInfo info;
			p >> &info;
			if (netman->isServer()) //server
			{
				switch (info.type)
				{
					case hash("Paddle"):
					{
						createEntity<Paddle>(ci->id);
					} break;
					case hash("Ball"):
					{
						createEntity<Ball>(ci->id);
					} break;
				}
			}
			else
			{
				switch (info.type)
				{
					//issue with constructor data, oh no
					case hash("Paddle"):
					{
						entities.emplace_back(std::make_unique<Paddle>());
					} break;
					case hash("Ball"):
					{
						entities.emplace_back(std::make_unique<Ball>());
					} break;
				}

				entities.back()->entity_info = info;
			}
		} break;
		case hash("ClientID"):
		{
			createEntity<Paddle>();
		}
	}
}

Entity* NetworkingState::getEntity(uint32_t networkID)
{
	auto& it = std::find_if(entities.begin(), entities.end(), [networkID](const auto& ent)
	{
		return ent->entity_info.networkID == networkID;
	});

	if (it != entities.end())
	{
		return it->get();
	}

	return nullptr;
}
