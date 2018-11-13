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
	{
		player_host = Host{ip_address};	
		Snapshot dummy_s = {};
		UserInput dummy_i = {};
		snapshots[client] = dummy_s;
		inputs[client] = dummy_i;
	}
			

	if (player_host.client())
	{		
		player_host.send(inputs[client]);
		player_host.receive(snapshots[client]);
		inputs[client].ack = snapshots[client].seq;
	}
	else
	{	
		player_host.send(snapshots);
		player_host.receive(inputs);
	}

	player_id = state.player_id;
}

}
