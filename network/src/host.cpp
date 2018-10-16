#include "host.hpp"

namespace network
{

Host::Host(const char* ip_address)
{
	enet_host = enet_host_create(nullptr, 1, 2, 0, 0);
	enet_address_set_host(&address, ip_address);
	address.port = 1234;
}

Host::Host()
{
	address.host = ENET_HOST_ANY;
	address.port = 1234;
	enet_host = enet_host_create(&address, 32, 2, 0, 0);
}

Host::~Host()
{
	enet_host_destroy(enet_host);
}

bool Host::connected() const
{
	for (auto* peer : peers)
		if (peer)
			return true;
		
	return false;
}

void Host::update(GameState& state)
{
	send(state);
	recieve(state);
}

void Host::send(GameState& state)
{
	for (auto* peer : peers)
	{
		if (peer)
		{
			ENetPacket* enet_packet =
				enet_packet_create(&state, 
				sizeof(GameState) + 1,
				ENET_PACKET_FLAG_UNSEQUENCED
				| ENET_PACKET_FLAG_NO_ALLOCATE);
			enet_peer_send(peer, 0, enet_packet);
		}
	}
}

void Host::recieve(GameState& state)
{
	ENetEvent event;
	while (enet_host_service(enet_host, &event, 0) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT: connect(event); break;
		case ENET_EVENT_TYPE_DISCONNECT: disconnect(event); break;
		}

		state = *reinterpret_cast<GameState*>(event.packet->data);
		enet_packet_destroy(event.packet);
	}
}

void Host::connect(const ENetEvent& event)
{
	++num_players;
	for (auto& peer : peers)
		if (peer == nullptr)
			peer = event.peer;
}

void Host::disconnect(const ENetEvent& event)
{
	--num_players;
	for (auto& peer : peers)
		if (peer == event.peer)
			peer = nullptr;
}


}

