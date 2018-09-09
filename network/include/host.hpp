#ifndef HOST_HPP
#define HOST_HPP

#include <functional>
#include <iostream>
#include <chrono>
#include <string>
#include <enet/enet.h>
#include "packet.hpp"

inline void net_init() { enet_initialize(); }
inline void net_uninit() { enet_deinitialize(); }

class host
{
public:
	virtual ~host() = default;
	virtual void update(const packet& p) = 0;
};


class client : public host
{
public:
	client(const std::string& ip_address);
	~client();

	void update(const packet& p) override;

	std::string msg = "receive";

private:
	void connect(const ENetEvent& event);
	void recieve(const ENetEvent& event);
	void disconnect(const ENetEvent& event);

	ENetAddress address;
	ENetHost* enet_host;
	ENetPeer* peer;
};

class server : public host
{
public:
	server();
	~server();

	void update(const packet& p) override;

	std::string msg = "receive";

private:
	void connect(const ENetEvent& event);
	void recieve(const ENetEvent& event);
	void disconnect(const ENetEvent& event);

	ENetAddress address;
	ENetHost* enet_host;
	ENetPeer* peers[10];
	int num_peers = 0;
};

#endif

