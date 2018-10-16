#ifndef HOST_HPP
#define HOST_HPP

#include <enet/enet.h>
#include "packet.hpp"

namespace network
{

class EnetInit
{
public:
	EnetInit();
	~EnetInit();
};

class Host
{
public:
	Host(const char* ip_address);
	Host();
	~Host();

	Host& operator=(const Host& other);

	bool connected() const;

	void update(GameState& state);

	int num_players = 1;

private:
	void send(GameState& state);
	void receive(GameState& state);

	void connect(const ENetEvent& event);
	void disconnect(const ENetEvent& event);

	ENetAddress address;
	ENetHost* enet_host;
	ENetPeer* peers[4] = {nullptr};
};

}

#endif

