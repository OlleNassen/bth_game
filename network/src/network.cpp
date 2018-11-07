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
		player_host = Host{ip_address};		

	if (player_host.client())
	{
		GameInput send_data;
		send_data.data = state.inputs[player_id];
		send_data.id = player_id;
		
		player_host.send(send_data);
		player_host.receive(state);
	}
	else
	{	
		player_host.send(state);
		player_host.receive(state.inputs);
	}

	player_id = state.player_id;
}

}
