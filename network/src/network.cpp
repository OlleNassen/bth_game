#include "network.hpp"

namespace network
{

Messenger::Messenger()
{
	enet_initialize();
}
Messenger::~Messenger()
{
	enet_deinitialize();
}

bool Messenger::connected() const 
{ 
	return player_host.connected(); 
}

void Messenger::update(GameState& state, const char* ip_address)
{	
	if (ip_address)
		player_host = Host{ip_address};

	write_state(state);
	player_host.update(state);
	read_state(state);
}

void Messenger::write_state(GameState& state)
{

}

void Messenger::read_state(GameState& state)
{

}

}
