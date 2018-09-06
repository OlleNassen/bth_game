#include "host.hpp"

#include <iostream>
using std::cout;
using std::cin;
using std::string;
constexpr char nl = '\n';

client::client()
{
	host = enet_host_create(nullptr, 1, 2, 0, 0);
	enet_address_set_host(&address, "192.168.43.205");
	address.port = 1234;
	peer = enet_host_connect(host, &address, 2, 0);

	if (peer)
	{
		ENetEvent event;
		if (enet_host_service(host, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
		{
			cout << "Connection to 192.168.0.195:1234 succeeded." << nl;
		}
		else
		{
			enet_peer_reset(peer);
			cout << "Connection to 192.168.0.195:1234 failed." << nl;
		}
	}
}

client::~client()
{
	enet_host_destroy(host);
}

void client::update()
{
	/* Create a reliable packet of size 7 containing "packet\0" */
	std::string s = "packet";
	ENetPacket * packet = enet_packet_create(s.c_str(), s.length() + 1, ENET_PACKET_FLAG_RELIABLE);

	/* Send the packet to the peer over channel id 0. */
	enet_peer_send(peer, 0, packet);
	
	using namespace std::chrono_literals;
	host_service(1000ms, host, [this]{ connect(); }, [this]{ recieve(); }, [this]{ disconnect(); });
}

void client::connect()
{

}

void client::recieve()
{

}

void client::disconnect()
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

void server::update()
{
	using namespace std::chrono_literals;
	host_service(1000ms, host, [this] { connect(); }, [this] { recieve(); }, [this] { disconnect(); });
}

void server::connect()
{

}

void server::recieve()
{
	cout << "Recieving" << nl;
}

void server::disconnect()
{

}

void test_net()
{
	if (enet_initialize() != 0)
		std::cout << "Error while initializing ENet" << '\n';

	int i = 0;
	std::cin >> i;

	if (i == 0)
	{
		server s;
		while (true) s.update();
	}
	else
	{
		client c;
		while (true) c.update();
	}

	enet_deinitialize();
}

