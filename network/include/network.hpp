#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <array>
#include <vector>
#include <glm/glm.hpp>

#include "host.hpp"
#include "packet.hpp"

namespace network
{

class Messenger
{
public:
	Messenger();
	~Messenger();

	bool connected() const;
	
	void update(GameState& state, const char* ip_address);

private:
	void write_state(GameState& state);
	void read_state(GameState& state);

	queue packet_queue;
	Host player_host;
	int player_id = 0;
	int num_players = 1;
	float connect_timer{};
	float disconnect_timer{};
};

}
#endif

