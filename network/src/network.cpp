#include "network.hpp"

namespace network
{

Messenger::Messenger()
{
	enet_initialize();
}
Messenger::~Messenger()
{
	enet_deinitialize();
}

Output Messenger::update(Input input)
{	
	std::array<glm::vec2, 4> pos;
	std::array<glm::vec2, 4> vel;

	for (auto i = 0; i < 4; ++i)
	{
		pos[i] = input.positions[i];
		vel[i] = input.velocities[i];
	}
		
	
	player_data net_data = { 0, 1, input.directions, pos, vel };

	if (!host && !input.chat.empty())
	{
		if (input.chat == "server")
		{
			host = std::make_unique<Server>();
		}
		else
		{
			host = std::make_unique<Client>(input.chat);
		}
	}
	else if (host)
	{
		host->update(&net_data);
	}

	net_data.positions[net_data.player_id] = input.positions[net_data.player_id];
	net_data.velocities[net_data.player_id] = input.velocities[net_data.player_id];

	return { net_data.player_id, net_data.player_count, net_data.directions, net_data.positions, net_data.velocities };
}

}
