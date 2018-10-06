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
	player_data net_data = { 0, 1, input.directions };

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

	return { net_data.player_id, net_data.player_count, net_data.directions };
}

}
