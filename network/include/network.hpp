#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <map>
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
	snapshot_map snapshots;
	input_map inputs;
	EnetInit init;
	Host player_host;
	int player_id = 0;

	static constexpr enet_uint32 client = 0;
};

}
#endif

