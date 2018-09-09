#ifndef HOST_HPP
#define HOST_HPP

#include <functional>
#include <iostream>
#include <chrono>
#include <string>
#include <enet/enet.h>
#include "packet.hpp"

class client
{
public:
	client();
	~client();

	void update(const packet& p);

	std::string msg = "receive";

private:
	void connect(const ENetEvent& event);
	void recieve(const ENetEvent& event);
	void disconnect(const ENetEvent& event);

	ENetAddress address;
	ENetHost* host;
	ENetPeer* peer;
};

class server
{
public:
	server();
	~server();

	void update(const packet& p);

	std::string msg = "receive";

private:
	void connect(const ENetEvent& event);
	void recieve(const ENetEvent& event);
	void disconnect(const ENetEvent& event);

	ENetAddress address;
	ENetHost* host;
	ENetPeer* peers[10];
	int num_peers = 0;
};

void test_net();

#endif

