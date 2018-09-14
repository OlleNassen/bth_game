#ifndef HOST_HPP
#define HOST_HPP

#include <array>
#include <functional>
#include <iostream>
#include <chrono>
#include <string>
#include <enet/enet.h>
#include "packet.hpp"

inline void net_init() { enet_initialize(); }
inline void net_deinit() { enet_deinitialize(); }

template <typename F1, typename F2, typename F3>
void host_service(std::chrono::milliseconds time, ENetHost* h,
	F1 recieve,
	F2 connect,
	F3 disconnect)
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

class Host
{
public:
	virtual ~Host() = default;
	virtual void update(const packet& p, input* begin, input* end) = 0;
	virtual int id() const = 0;
};


class Client : public Host
{
public:
	Client(const std::string& ip_address);
	~Client();

	void update(const packet& p, input* begin, input* end) override;
	int id() const override { return client_id; }
	

private:
	void recieve(const ENetEvent& event, input* begin, input* end);
	void connect(const ENetEvent& event);
	void disconnect(const ENetEvent& event);	
	
	int client_id = 0;

	ENetAddress address;
	ENetHost* enet_host;
	ENetPeer* peer;
};

class Server : public Host
{
public:
	Server();
	~Server();

	void update(const packet& p, input* begin, input* end) override;
	int id() const override { return server_id; }

private:
	void recieve(const ENetEvent& event, input* begin, input* end);
	void connect(const ENetEvent& event);
	void disconnect(const ENetEvent& event);
	
	int server_id = 3;

	ENetAddress address;
	ENetHost* enet_host;
	ENetPeer* peers[10] = { nullptr };
	int num_peers = 0;
};

#endif

