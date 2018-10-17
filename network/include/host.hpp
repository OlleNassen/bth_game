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

	Host(const Host& other);
	Host& operator=(const Host& other);

	bool connected() const;

	void send(uint16 input);
	void send(GameState& state);
	void receive(uint64& input);
	void receive(GameState& state);

private:
	void connect(const ENetEvent& event);
	void disconnect(const ENetEvent& event);

	uint8 player_count = 1;
	uint32 sequence = 0;
	
	ENetAddress address;
	ENetHost* enet_host;
	ENetPeer* peers[4] = {nullptr};	
};

}

#endif

