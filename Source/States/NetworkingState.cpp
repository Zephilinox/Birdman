#include "NetworkingState.hpp"
//SELF
#include "../Architecture/GameData.hpp"
#include "../States/MenuState.hpp"
#include "../States/FadeOutState.hpp"

NetworkingState::NetworkingState(GameData* game_data)
	: BaseState(game_data)
{
	enetpp::global_state::get().initialize();
	std::cout << "server is 0, client is 1: ";
	std::cin >> type;
	std::cout << "\n";

	if (type == 0)
	{
		auto init_client_func = [&](ServerClient& client, const char* ip)
		{
			client.id = next_uid;
			next_uid++;
		};

		server.start_listening(enetpp::server_listen_params<ServerClient>()
			.set_max_client_count(20)
			.set_channel_count(1)
			.set_listen_port(11111)
			.set_initialize_client_function(init_client_func));
	}
	else if (type == 1)
	{
		client.connect(enetpp::client_connect_params()
			.set_channel_count(1)
			.set_server_host_name_and_port("localhost", 11111));
	}
}

void NetworkingState::update(const ASGE::GameTime& gt)
{
	if (type == 0)
	{
		updateServer();
	}
	else if (type == 1)
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
	std::string data;
	std::cout << "server: ";
	std::getline(std::cin, data);
	std::cout << "\n";

	server.send_packet_to_all_if(0, (enet_uint8*)(data.c_str()), data.size(), ENET_PACKET_FLAG_RELIABLE, [](const ServerClient&) {return true; });

	auto on_client_connected = [&](ServerClient& client) {};
	auto on_client_disconnected = [&](unsigned int client_uid) {};
	auto on_client_data_received = [&](ServerClient& client, const enet_uint8* data, size_t data_size)
	{
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
	std::string data;

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
