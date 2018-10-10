#include "host.hpp"

#include <iostream>
using std::cout;
using std::cin;
using std::string;
constexpr char nl = '\n';

Client::Client(const std::string& ip_address)
{
	enet_host = enet_host_create(nullptr, 1, 2, 0, 0);
	enet_address_set_host(&address, ip_address.c_str());
	address.port = 1234;
}

Client::~Client()
{
	enet_host_destroy(enet_host);
}

void Client::update(player_data* data)
{
	ENetPacket* enet_packet = enet_packet_create(data, sizeof(glm::vec3) * 4 + 1,
		ENET_PACKET_FLAG_UNSEQUENCED | ENET_PACKET_FLAG_NO_ALLOCATE);

	if (!peer)
	{
		peer = enet_host_connect(enet_host, &address, 2, 0);
	}
	else if (connected)
	{
		enet_peer_send(peer, 0, enet_packet);
	}

	using namespace std::chrono_literals;
	host_service(0ms, enet_host,
		[this, data](const ENetEvent& event) { recieve(event, data); },
		[this](const ENetEvent& event) { connect(event); },
		[this](const ENetEvent& event) { disconnect(event); });
}

void Client::recieve(const ENetEvent& event, player_data* data)
{
	const auto* new_data = reinterpret_cast<player_data*>(event.packet->data);
	data->player_count = new_data->player_count;
	data->player_id = new_data->player_id;
	
	for (int i = 1; i < 4; ++i)
	{
		data->directions[i] = new_data->directions[i];
		data->positions[i] = new_data->positions[i];
		data->velocities[i] = new_data->velocities[i];
	}
}

void Client::connect(const ENetEvent& event)
{
	cout << "Connection succeeded." << nl;
	peer = event.peer;
	connected = true;
}

void Client::disconnect(const ENetEvent& event)
{

}

Server::Server()
{
	address.host = ENET_HOST_ANY;
	address.port = 1234;

	enet_host = enet_host_create(&address, 32, 2, 0, 0);
	connected = true;
}

Server::~Server()
{
	enet_host_destroy(enet_host);
}

void Server::update(player_data* data)
{	
	data->player_count = num_peers + 1;

	/* Send the packet to the peer over channel id 0. */
	for (int i = 0; i < 3; ++i)
	{
		auto* peer = peers[i];
		if (peer)
		{
			data->player_id = i + 1;

			ENetPacket* enet_packet = enet_packet_create(data, sizeof(player_data) + 1,
				ENET_PACKET_FLAG_UNSEQUENCED | ENET_PACKET_FLAG_NO_ALLOCATE);
			enet_peer_send(peer, 0, enet_packet);

			enet_host_flush(enet_host);
		}		
	}
	data->player_id = 0;
	
	using namespace std::chrono_literals;
	host_service(0ms, enet_host,
		[this, data](const ENetEvent& event) { recieve(event, data); },
		[this](const ENetEvent& event) { connect(event); },
		[this](const ENetEvent& event) { disconnect(event); });	
}

void Server::recieve(const ENetEvent& event, player_data* data)
{
	const auto* new_data = reinterpret_cast<player_data*>(event.packet->data);

	auto index = new_data->player_id;
	data->directions[index] = new_data->directions[index];
	data->positions[index] = new_data->positions[index];
	data->velocities[index] = new_data->velocities[index];
}

void Server::connect(const ENetEvent& event)
{
	if (num_peers < 3)
	{
		cout << "Player " << ++num_peers + 1 << " connected." << nl;
		using namespace std;
		auto& peer = *find(begin(peers), end(peers), nullptr);
		peer = event.peer;
	}
	else
	{
		cout << "Game is full. :(" << nl;
	}
	
}

void Server::disconnect(const ENetEvent& event)
{
	cout << "Player " << num_peers-- + 1 << " disconnected." << nl;
	using namespace std;
	auto& peer = *find(begin(peers), end(peers), event.peer);
	peer = nullptr;
}

