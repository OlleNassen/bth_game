#ifndef HOST_HPP
#define HOST_HPP

#include <array>
#include <functional>
#include <iostream>
#include <chrono>
#include <string>
#include <glm/glm.hpp>
#include <enet/enet.h>
#include "packet.hpp"

struct player_data
{
	int player_id = 0;
	int player_count = 1;
	glm::vec3 directions[4];
};

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
		case ENET_EVENT_TYPE_RECEIVE: recieve(event); enet_packet_destroy(event.packet); break;
		case ENET_EVENT_TYPE_CONNECT: connect(event); break;
		case ENET_EVENT_TYPE_DISCONNECT: disconnect(event); break;
		}
	}
}

class Host
{
public:
	virtual ~Host() = default;
	virtual void update(player_data* data) = 0;

	bool connected = false;
};


class Client : public Host
{
public:
	Client(const std::string& ip_address);
	~Client();

	void update(player_data* data) override;
	

private:
	void recieve(const ENetEvent& event, player_data* data);
	void connect(const ENetEvent& event);
	void disconnect(const ENetEvent& event);	

	ENetAddress address;
	ENetHost* enet_host;
	ENetPeer* peer = nullptr;
};

class Server : public Host
{
public:
	Server();
	~Server();

	void update(player_data* data) override;

private:	
	void recieve(const ENetEvent& event, player_data* data);
	void connect(const ENetEvent& event);
	void disconnect(const ENetEvent& event);	

	ENetAddress address;
	ENetHost* enet_host;
	ENetPeer* peers[10] = { nullptr };
	int num_peers = 0;
};

#endif

