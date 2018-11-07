#include "host.hpp"
#include <iostream>

namespace network
{

EnetInit::EnetInit()
{
	enet_initialize();
}

EnetInit::~EnetInit()
{
	enet_deinitialize();
}

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
	for (auto* peer : peers)
	{
		if (peer)
		{
			enet_peer_disconnect(peer, 0);
		}
	}

	enet_host_flush(enet_host);
	enet_host_destroy(enet_host);
}

Host::Host(const Host& other)
{
	enet_host = enet_host_create(nullptr, 1, 2, 0, 0);
	address = other.address;
	enet_host_connect(enet_host, &address, 2, 0);
}

Host& Host::operator=(const Host& other)
{
	enet_host_destroy(enet_host);
	enet_host = enet_host_create(nullptr, 1, 2, 0, 0);
	address = other.address;
	for (auto& peer : peers)
		peer = nullptr;

	enet_host_connect(enet_host, &address, 2, 0);
	is_client = true;

	return *this;
}

bool Host::connected() const
{
	for (auto* peer : peers)
		if (peer)
			return true;

	return false;
}

bool Host::client() const
{
	return is_client;
}

void Host::send(GameInput& input)
{
	if (enet_host)
	{
		auto* peer = peers[0];
		if (peer)
		{
			ENetPacket* enet_packet =
				enet_packet_create(&input,
					sizeof(GameInput),
					ENET_PACKET_FLAG_UNSEQUENCED
					| ENET_PACKET_FLAG_NO_ALLOCATE);
			enet_peer_send(peer, 0, enet_packet);
			enet_host_flush(enet_host);
		}
	}
}

void Host::send(GameState& state)
{
	if (enet_host)
	{
		state.sequence = ++sequence;
		state.player_count = player_count;

		int index = 0;
		for (auto* peer : peers)
		{
			if (peer)
			{
				state.player_id = ++index;
				ENetPacket* enet_packet =
					enet_packet_create(&state,
						sizeof(GameState),
						ENET_PACKET_FLAG_UNSEQUENCED
						| ENET_PACKET_FLAG_NO_ALLOCATE);
				enet_peer_send(peer, 0, enet_packet);
				enet_host_flush(enet_host);
			}
		}
		state.player_id = 0;
	}
}

void Host::receive(logic::input* input)
{
	if (enet_host)
	{
		ENetEvent eevent;
		while (enet_host_service(enet_host, &eevent, 0) > 0)
		{
			switch (eevent.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
			{
				GameInput g = *reinterpret_cast<GameInput*>(eevent.packet->data);
				input[g.id] = g.data;
				break;
			}
			case ENET_EVENT_TYPE_CONNECT: connect(eevent); break;
			case ENET_EVENT_TYPE_DISCONNECT: disconnect(eevent); break;
			}
			enet_packet_destroy(eevent.packet);
		}
	}
}

void Host::receive(GameState& state)
{
	if (enet_host)
	{
		ENetEvent eevent;
		while (enet_host_service(enet_host, &eevent, 0) > 0)
		{
			switch (eevent.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
			{
				state = *reinterpret_cast<GameState*>(eevent.packet->data);
				break;
			}			
			case ENET_EVENT_TYPE_CONNECT: connect(eevent); break;
			case ENET_EVENT_TYPE_DISCONNECT: disconnect(eevent); break;
			}
			enet_packet_destroy(eevent.packet);
		}
	}
}

void Host::connect(const ENetEvent& eevent)
{
	std::cout << "Connected." << '\n';
	++player_count;
	for (auto& peer : peers)
	{
		if (peer == nullptr)
		{
			peer = eevent.peer;
			break;
		}
	}			
}

void Host::disconnect(const ENetEvent& eevent)
{
	std::cout << "Diconnected." << '\n';
	--player_count;
	for (auto& peer : peers)
	{
		if (peer == eevent.peer)
		{
			peer = nullptr;
			break;
		}
	}			
}


}
