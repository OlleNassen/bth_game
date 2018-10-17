#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "host.hpp"
#include "packet.hpp"

namespace network
{

class Messenger
{
public:
	Messenger() : init{}, player_host{} {}

	int connected_players() const;
	int id() const;
	bool connected() const;
	void update(GameState& state, const char* ip_address);

private:
	EnetInit init;
	Host player_host;
	int num_players = 1;
	int player_id = 0;
};

}
#endif

