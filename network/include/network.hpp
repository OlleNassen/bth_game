#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "host.hpp"
#include "packet.hpp"

namespace network
{

class Messenger
{
public:
	int id() const;
	bool connected() const;
	void update(GameState& state, const char* ip_address);

private:
	EnetInit init;
	Host player_host;
	int player_id = 0;
};

}
#endif

