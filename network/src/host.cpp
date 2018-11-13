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

void Host::send(const UserInput& input)
{	
	if (enet_host && peers.size() == 1u)
	{
		uint32 data[320] = {};
		bit_writer writer{data , 100 / 32};
		writer << input;

		ENetPacket* enet_packet = enet_packet_create(
			writer.data(),
			writer.size(),
			ENET_PACKET_FLAG_UNSEQUENCED | ENET_PACKET_FLAG_NO_ALLOCATE);
		enet_peer_send(peers.front(), 0, enet_packet);
		enet_host_flush(enet_host);
	}
}

void Host::receive(Snapshot& snapshot)
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
				recieve(eevent, snapshot);
				break;
			}
			case ENET_EVENT_TYPE_CONNECT:
			{
				connect(eevent);
				break;
			}
			case ENET_EVENT_TYPE_DISCONNECT:
			{
				disconnect(eevent);
				break;
			}
			}
			enet_packet_destroy(eevent.packet);
		}
	}
}

void Host::send(snapshot_map& snapshots)
{
	if (enet_host)
	{
		for (const auto& peer : peers)
		{
			uint32 data[320] = {};
			bit_writer writer{data , 100 / 32};
			writer << snapshots[peer->connectID];

			ENetPacket* enet_packet = enet_packet_create(
				writer.data(),
				writer.size(),
				ENET_PACKET_FLAG_UNSEQUENCED | ENET_PACKET_FLAG_NO_ALLOCATE);
			enet_peer_send(peer, 0, enet_packet);
			enet_host_flush(enet_host);
		}
	}
}

void Host::receive(input_map& input)
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
				recieve(eevent, input[eevent.peer->connectID]);
				break;
			}
			case ENET_EVENT_TYPE_CONNECT:
			{
				connect(eevent);
				break;
			}
			case ENET_EVENT_TYPE_DISCONNECT: 
			{
				disconnect(eevent);
				break;
			}
			}
			enet_packet_destroy(eevent.packet);
		}
	}
}

void Host::recieve(const ENetEvent& eevent, UserInput& input)
{
	uint32* data = reinterpret_cast<uint32*>(eevent.packet->data);
	int size = static_cast<int>(eevent.packet->dataLength / 32);
	bit_reader reader{data, size};
	reader >> input;
}

void Host::recieve(const ENetEvent& eevent, Snapshot& snapshot)
{
	uint32* data = reinterpret_cast<uint32*>(eevent.packet->data);
	int size = static_cast<int>(eevent.packet->dataLength / 32);
	bit_reader reader{data, size};
	reader >> snapshot;
}

void Host::connect(const ENetEvent& eevent)
{
	std::cout << "Connected." << '\n';
	peers.push_back(eevent.peer);
}

void Host::disconnect(const ENetEvent& eevent)
{
	std::cout << "Diconnected." << '\n';
	auto position = std::find(peers.begin(), peers.end(), eevent.peer);
	if (position != peers.end()) peers.erase(position);
}


}
