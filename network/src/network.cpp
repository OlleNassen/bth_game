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
		player_host.send(state.inputs[player_id]);
		player_host.receive(state);
	}
	else
	{
		player_host.send(state);
		for (auto& input : state.inputs)
		{
			player_host.receive(input);
		}		
	}
}

}
