#include "stdafx.h"
#include "NetworkManager.hpp"

NetworkManager::NetworkManager(GameData* game_data)
	: game_data(game_data)
{
	enetpp::global_state::get().initialize();
}

NetworkManager::~NetworkManager()
{
	reset();
	enetpp::global_state::get().deinitialize();
}

void NetworkManager::initialize(bool hostServer)
{
	initialized = true;
	hostingServer = hostServer;

	if (hostingServer)
	{
		auto client_init = [&](ClientInfo& client, const char* ip)
		{
			std::cout << "Client " << next_uid << " initialized with IP " << ip << "\n";
			client.id = next_uid;
			next_uid++;
			
			//Tell the client what its ID is
			Packet p;
			p.setID(hash("ClientID"));
			p << client.id;
			sendPacket(client.id, 0, &p);
		};

		server.start_listening(enetpp::server_listen_params<ClientInfo>()
			.set_max_client_count(2)
			.set_channel_count(2)
			.set_listen_port(11111)
			.set_initialize_client_function(client_init));
	}
	else
	{
		client.connect(enetpp::client_connect_params()
			.set_channel_count(2)
			.set_server_host_name_and_port("localhost", 11111));
	}
}

void NetworkManager::reset()
{
	if (hostingServer)
	{
		server.stop_listening();
	}
	else
	{
		client.disconnect();
	}

	initialized = false;
}

void NetworkManager::update()
{
	if (initialized)
	{
		if (hostingServer)
		{
			updateServer();
		}
		else
		{
			updateClient();
		}
	}
}

void NetworkManager::sendPacket(enet_uint8 channel_id, Packet* p, enet_uint32 flags)
{
	if (!hostingServer)
	{
		client.send_packet(channel_id, p->buffer.data(), p->buffer.size(), flags);
	}
	else
	{
		sendPacket(channel_id, p, flags, [](const ClientInfo& client) {return true; });
	}
}

void NetworkManager::sendPacket(uint32_t client_id, enet_uint8 channel_id, Packet* p, enet_uint32 flags)
{
	assert(hostingServer);
	server.send_packet_to(client_id, channel_id, p->buffer.data(), p->buffer.size(), flags);
}

void NetworkManager::sendPacket(enet_uint8 channel_id, Packet* p, enet_uint32 flags, std::function<bool(const ClientInfo& client)> predicate)
{
	assert(hostingServer);
	server.send_packet_to_all_if(channel_id, p->buffer.data(), p->buffer.size(), flags, predicate);
}

bool NetworkManager::isServer()
{
	return hostingServer;
}

bool NetworkManager::isConnected()
{
	return client.is_connecting_or_connected() && clientConnectedToServer;
}

bool NetworkManager::isInitialized()
{
	return initialized;
}

void NetworkManager::updateServer()
{
	auto on_client_connected = [&](ClientInfo& client)
	{
		std::cout << "client " << client.id << " connected \n";
		client_connected.emit(&client);
	};

	auto on_client_disconnected = [&](uint32_t client_uid)
	{
		client_disconnected.emit(client_uid);
	};

	auto on_client_data_received = [&](const enet_uint8 channel_id, ClientInfo& client, const enet_uint8* data, size_t data_size)
	{
		client_sent_packet.emit(std::move(channel_id), &client, {data, data_size});
	};

	server.consume_events(on_client_connected, on_client_disconnected, on_client_data_received);
}

void NetworkManager::updateClient()
{
	auto on_connected = [&]()
	{
		clientConnectedToServer = true;
		connected.emit();
	};

	auto on_disconnected = [&]()
	{
		clientConnectedToServer = false;
		disconnected.emit();
	};

	auto on_data_received = [&](const enet_uint8 channel_id, const enet_uint8* data, size_t data_size)
	{
		Packet p(data, data_size);
		if (p.getID() != hash("ClientID"))
		{
			server_sent_packet.emit(std::move(channel_id), std::move(p));
		}
		else
		{
			p >> clientID;
			std::cout << "ClientID " << clientID << " received\n";
		}
	};

	client.consume_events(on_connected, on_disconnected, on_data_received);
}
