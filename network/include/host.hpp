#ifndef HOST_HPP
#define HOST_HPP

#include <vector>
#include <enet/enet.h>
#include "packet.hpp"

namespace network
{

using snapshot_map = std::map<enet_uint32, Snapshot>;
using input_map = std::map<enet_uint32, UserInput>;

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

	void send(UserInput& input);
	void receive(Snapshot& snapshot);
	
	void send(snapshot_map& snapshots);
	void receive(input_map& input);

private:
	void recieve(const ENetEvent& eevent, UserInput& input);
	void recieve(const ENetEvent& eevent, Snapshot& snapshot);
	void connect(const ENetEvent& eevent);
	void disconnect(const ENetEvent& eevent);

	uint8 player_count = 1;
	uint32 sequence = 0;

	ENetAddress address;
	ENetHost* enet_host;
	std::vector<ENetPeer*> peers;

	bool is_client = false;
};

}

#endif


