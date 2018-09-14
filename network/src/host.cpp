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
	peer = enet_host_connect(enet_host, &address, 2, 0);

	if (peer)
	{
		ENetEvent event;
		if (enet_host_service(enet_host, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
		{
			cout << "Connection succeeded." << nl;
		}
		else
		{
			enet_peer_reset(peer);
			cout << "Connection failed." << nl;
		}
	}
}

Client::~Client()
{
	enet_host_destroy(enet_host);
}

void Client::update(const Packet& p, input* begin, input* end)
{
	ENetPacket* enet_packet = enet_packet_create(p.data(), p.size() + 1,
		ENET_PACKET_FLAG_UNSEQUENCED | ENET_PACKET_FLAG_NO_ALLOCATE);

	/* Send the packet to the peer over channel id 0. */
	enet_peer_send(peer, 0, enet_packet);

	using namespace std::chrono_literals;
	host_service(0ms, enet_host,
		[this, begin, end](const ENetEvent& event) { recieve(event, begin, end); },
		[this](const ENetEvent& event) { connect(event); },
		[this](const ENetEvent& event) { disconnect(event); });
}

void Client::recieve(const ENetEvent& event, input* begin, input* end)
{
	const input* data = reinterpret_cast<input*>(event.packet->data);
	const auto* index = &data[client_id];
	
	for (int i = 0; i < (end - begin); ++i)
	{
		if (&data[i] != index)
		{
			begin[i] = data[i];
		}
	}
}

void Client::connect(const ENetEvent& event)
{
	peer = event.peer;
	client_id = event.peer->connectID;
}

void Client::disconnect(const ENetEvent& event)
{

}

Server::Server()
{
	address.host = ENET_HOST_ANY;
	address.port = 1234;

	enet_host = enet_host_create(&address, 32, 2, 0, 0);
}

Server::~Server()
{
	enet_host_destroy(enet_host);
}

void Server::update(const Packet& p, input* begin, input* end)
{
	ENetPacket* enet_packet = enet_packet_create(p.data(), p.size() + 1,
		ENET_PACKET_FLAG_UNSEQUENCED | ENET_PACKET_FLAG_NO_ALLOCATE);

	/* Send the packet to the peer over channel id 0. */
	for (auto* peer : peers)
	{
		if (peer) enet_peer_send(peer, 0, enet_packet);	
	}

	using namespace std::chrono_literals;
	host_service(0ms, enet_host,
		[this, &p, begin, end](const ENetEvent& event) { recieve(event, begin, end); },
		[this, &p](const ENetEvent& event) { connect(event); },
		[this, &p](const ENetEvent& event) { disconnect(event); });
}

void Server::recieve(const ENetEvent& event, input* begin, input* end)
{
	const input* data = reinterpret_cast<input*>(event.packet->data);
	const auto* index = &data[server_id];

	for (int i = 0; i < (end - begin); ++i)
	{
		if (&data[i] != index)
		{
			begin[i] = data[i];
		}
	}
}

void Server::connect(const ENetEvent& event)
{
	peers[num_peers++] = event.peer;
}

void Server::disconnect(const ENetEvent& event)
{

}

