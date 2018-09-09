#include "host.hpp"

#include <iostream>
using std::cout;
using std::cin;
using std::string;
constexpr char nl = '\n';

void host_service(std::chrono::milliseconds time, ENetHost* h,
	std::function<void(const ENetEvent&)> recieve,
	std::function<void(const ENetEvent&)> connect,
	std::function<void(const ENetEvent&)> disconnect)
{
	ENetEvent event;
	while (enet_host_service(h, &event, static_cast<enet_uint32>(time.count())) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_RECEIVE: recieve(event); break;
		case ENET_EVENT_TYPE_CONNECT: connect(event); break;
		case ENET_EVENT_TYPE_DISCONNECT: disconnect(event); break;
		}
	}
}

client::client()
{
	host = enet_host_create(nullptr, 1, 2, 0, 0);
	enet_address_set_host(&address, "192.168.1.114");
	address.port = 1234;
	peer = enet_host_connect(host, &address, 2, 0);

	if (peer)
	{
		ENetEvent event;
		if (enet_host_service(host, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
		{
			cout << "Connection to 192.168.1.114:1234 succeeded." << nl;
		}
		else
		{
			enet_peer_reset(peer);
			cout << "Connection to 192.168.1.114:1234 failed." << nl;
		}
	}
}

client::~client()
{
	enet_host_destroy(host);
}

void client::update(const packet& p)
{
	std::cin >> msg;
	ENetPacket* enet_packet = enet_packet_create(msg.c_str(), msg.length() + 1, //p.data(), p.size() + 1, 
		ENET_PACKET_FLAG_UNSEQUENCED | ENET_PACKET_FLAG_NO_ALLOCATE);

	/* Send the packet to the peer over channel id 0. */
	enet_peer_send(peer, 0, enet_packet);

	using namespace std::chrono_literals;
	host_service(0ms, host,
		[this](const ENetEvent& event) { recieve(event); },
		[this](const ENetEvent& event) { connect(event); },
		[this](const ENetEvent& event) { disconnect(event); });
}

void client::connect(const ENetEvent& event)
{

}

void client::recieve(const ENetEvent& event)
{
	cout << msg << nl;
}

void client::disconnect(const ENetEvent& event)
{

}

server::server()
{
	address.host = ENET_HOST_ANY;
	address.port = 1234;

	host = enet_host_create(&address, 32, 2, 0, 0);
}

server::~server()
{
	enet_host_destroy(host);
}

void server::update(const packet& p)
{
	//std::cin >> msg;

	ENetPacket* enet_packet = enet_packet_create(msg.c_str(), msg.length() + 1, //p.data(), p.size() + 1, 
		ENET_PACKET_FLAG_UNSEQUENCED | ENET_PACKET_FLAG_NO_ALLOCATE);

	/* Send the packet to the peer over channel id 0. */
	for (auto* peer : peers)
	{
		//if (peer) enet_peer_send(peer, 0, enet_packet);	
	}

	using namespace std::chrono_literals;
	host_service(0ms, host,
		[this, &p](const ENetEvent& event) { recieve(event); },
		[this, &p](const ENetEvent& event) { connect(event); },
		[this, &p](const ENetEvent& event) { disconnect(event); });
}

void server::connect(const ENetEvent& event)
{
	peers[num_peers++] = event.peer;
}

void server::recieve(const ENetEvent& event)
{
	cout << event.packet->data << nl;
}

void server::disconnect(const ENetEvent& event)
{

}

void test_net()
{
	if (enet_initialize() != 0)
		std::cout << "Error while initializing ENet" << '\n';

	int i = 0;
	std::cin >> i;

	packet p;

	if (i == 0)
	{
		server s;
		while (true) s.update(p);
	}
	else
	{
		client c;
		while (true) c.update(p);
	}

	enet_deinitialize();
}

