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
	bool client() const;

	void send(GameInput& input);
	void send(GameState& state);
	void receive(logic::input* input);
	void receive(GameState& state);

private:
	void connect(const ENetEvent& eevent);
	void disconnect(const ENetEvent& eevent);

	uint8 player_count = 1;
	uint32 sequence = 0;

	ENetAddress address;
	ENetHost* enet_host;
	ENetPeer* peers[4] = { nullptr };

	bool is_client = false;
};

}

#endif


