#include "network.hpp"
#include <iostream>

namespace network
{

int Messenger::id() const
{
	return player_id;
}

bool Messenger::connected() const 
{ 
	return player_host.connected(); 
}

void Messenger::update(GameState& state, const char* ip_address)
{	
	if (ip_address)
	{
		player_host = Host{ip_address};
		++player_id;
	}

	if (player_id)
	{
		uint16 input = static_cast<uint16>(state.input >> (player_id * 16));
		player_host.send(input);
		player_host.receive(state);
		uint64 input64 = 0;
		input64 = input;
		state.input = (state.input | (input64 << (player_id * 16)));
	}
	else
	{
		player_host.send(state);
		player_host.receive(state.input);
	}
}

}
