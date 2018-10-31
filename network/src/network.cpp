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
	return true;// player_host.connected();
}

void Messenger::update(GameState& state, const char* ip_address)
{	
	if (player_id)
	{
		uint16 player_unput = state.inputs[player_id];
		player_host.send(player_unput);
		player_host.receive(state);
	}
	else
	{
		player_host.send(state);
		player_host.receive(state.inputs[1]);	
		
	}
	player_id = state.player_id;
}

}
