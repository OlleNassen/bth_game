#include "network.hpp"

namespace network
{

int Messenger::connected_players() const
{
	return num_players;
}

int Messenger::id() const
{
	return connected() ? player_host.player_id : 0;
}

bool Messenger::connected() const 
{ 
	return player_host.connected(); 
}

void Messenger::update(GameState& state, const char* ip_address)
{	
	if (ip_address)
		player_host = Host{ip_address};

	if (player_host.num_players > num_players)
	{
		++player_host.player_id;
	}
	else if (player_host.num_players < num_players)
	{
		--player_host.player_id;
	}
	num_players = player_host.num_players;

	player_host.update(state);
}

}
