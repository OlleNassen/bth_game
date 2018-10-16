#include "network.hpp"

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
	if (player_host.num_players > num_players)
	{
		++player_id;
	}
	else if (player_host.num_players < num_players)
	{
		--player_id;		
	}
	num_players = player_host.num_players;
}

}
