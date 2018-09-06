#ifndef HOST_HPP
#define HOST_HPP

#include <iostream>
#include <chrono>
#include <string>
#include <enet/enet.h>

template <typename T1, typename T2, typename T3>
void host_service(std::chrono::milliseconds time, ENetHost* h, T1 connect, T2 recieve, T3 disconnect)
{
	ENetEvent event;
	while (enet_host_service(h, &event, static_cast<enet_uint32>(time.count())) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT: connect(); break;
		case ENET_EVENT_TYPE_RECEIVE: recieve(); break;
		case ENET_EVENT_TYPE_DISCONNECT: disconnect(); break;
		}
	}
}

class client
{
public:
	client();
	~client();

	void update();

private:
	void connect();
	void recieve();
	void disconnect();

	ENetAddress address;
	ENetHost* host;
	ENetPeer* peer;
};

class server
{
public:
	server();
	~server();

	void update();

private:
	void connect();
	void recieve();
	void disconnect();

	ENetAddress address;
	ENetHost* host;
	ENetPeer* peer;
};

void test_net();

#endif

