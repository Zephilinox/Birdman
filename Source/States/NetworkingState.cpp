#include "NetworkingState.hpp"

//SELF
#include "../Architecture/GameData.hpp"
#include "../States/MenuState.hpp"
#include "../States/FadeOutState.hpp"

NetworkingState::NetworkingState(GameData* game_data)
	: BaseState(game_data)
{
	Packet p;
	p.serialize("Hello ");
	//doesn't get packed to char-sized int
	//results in 4 bytes packed, 1 valid, 3 null
	p.serialize(int('4'));
	p.serialize(int('2'));
	p.serialize(" World :)");
	char* data = (char*)p.buffer.data();
	int size = p.buffer.size();
	for (int i = 0; i < size; ++i)
	{
		std::cout << data[i];
	}

	std::cout << "\n Now for ints\n";
	Packet p2;
	p2.serialize(20180220); //the data, for funs.

	std::cout << "Here is the raw value of the int as ASCII\n";
	char* data2 = (char*)p2.buffer.data();
	int size2 = p2.buffer.size();
	for (int i = 0; i < size2; ++i)
	{
		std::cout << data2[i];
	}
	std::cout << "\nAnd here is the int value memcpy'd\n";

	int destination;
	memcpy(&destination, p2.buffer.data(), p2.buffer.size());
	std::cout << destination << "\n";

	std::cout << "\n";
	enetpp::global_state::get().initialize();
	int input;
	std::cout << "server is 0, client is 1: ";
	std::cin >> input;
	std::cout << "\n";

	if (input == 0)
	{
		isServer = true;
	}
	else
	{
		isServer = false;
	}
	
	if (isServer)
	{
		auto client_init = [&](ServerClient& client, const char* ip)
		{
			std::cout << "Client " << next_uid << " initialized with IP " << ip << "\n";
			client.id = next_uid;
			next_uid++;
		};

		server.start_listening(enetpp::server_listen_params<ServerClient>()
			.set_max_client_count(2)
			.set_channel_count(1)
			.set_listen_port(11111)
			.set_initialize_client_function(client_init));
	}
	else
	{
		client.connect(enetpp::client_connect_params()
			.set_channel_count(1)
			.set_server_host_name_and_port("localhost", 11111));
	}
}

NetworkingState::~NetworkingState()
{
	enetpp::global_state::get().deinitialize();
	server.stop_listening();
	client.disconnect();
}

void NetworkingState::update(const ASGE::GameTime& gt)
{
	if (isServer)
	{
		updateServer();
	}
	else if (!isServer)
	{
		updateClient();
	}
}

void NetworkingState::render() const
{

}

void NetworkingState::onActive()
{
}

void NetworkingState::onInactive()
{
}

void NetworkingState::updateServer()
{
	std::string data = "Hi";
	
	if (t.getElapsedTime() > 1)
	{
		t.restart();
		for (const auto& client : server.get_connected_clients())
		{
			server.send_packet_to(client->id, 0, (enet_uint8*)(data.c_str()), data.size(), ENET_PACKET_FLAG_RELIABLE);
		}
	}

	auto on_client_connected = [&](ServerClient& client)
	{
		std::cout << "client " << client.id << " connected \n";
	};
	auto on_client_disconnected = [&](unsigned int client_uid) {};
	auto on_client_data_received = [&](ServerClient& client, const enet_uint8* data, size_t data_size)
	{
		std::cout << "client " << client.id << ": ";
		for (int i = 0; i < data_size; ++i)
		{
			std::cout << data[i];
		}

		std::cout << "\n";
	};

	server.consume_events(on_client_connected, on_client_disconnected, on_client_data_received);
}

void NetworkingState::updateClient()
{
	if (t.getElapsedTime() > 1)
	{
		t.restart();
		std::string data = std::to_string(t2.getElapsedTime());
		client.send_packet(0, (enet_uint8*)(data.c_str()), data.size(), ENET_PACKET_FLAG_RELIABLE);
	}

	auto on_connected = [&]() {};
	auto on_disconnected = [&]() {};
	auto on_data_received = [&](const enet_uint8* data, size_t data_size)
	{
		for (int i = 0; i < data_size; ++i)
		{
			std::cout << data[i];
		}

		std::cout << "\n";
	};

	client.consume_events(on_connected, on_disconnected, on_data_received);
}
