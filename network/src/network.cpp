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
	return /*player_host.connected();*/ true;
}

void Messenger::update(GameState& state, const char* ip_address)
{
	if (ip_address)
		player_host = Host{ ip_address };		

	if (player_host.client())
	{
		uint16 player_input = state.inputs[player_id];
		player_host.send(player_input);
		player_host.receive(state);
	}
	else
	{	
		player_host.send(state);
		for (int i = 1; i < 4; ++i)
		{	
			player_host.receive(state.inputs[i], i);
		}
	}

	player_id = state.player_id;
	//std::cout << player_id << '\n';
}

}
