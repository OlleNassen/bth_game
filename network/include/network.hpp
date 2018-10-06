#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <memory>
#include <string>
#include <array>
#include <glm/glm.hpp>

#include "host.hpp"

namespace network
{

struct input
{
	const std::string& chat;
	const std::array<glm::vec3, 4>& directions;
};

struct output
{
	int player_id;
	int player_count;
	std::array<glm::vec3, 4> directions;
};

class n
{
public:
	n() { enet_initialize(); }
	~n() { enet_deinitialize(); }
	
	output update(input input)
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


private:
	std::unique_ptr<Host> host;
};

}
#endif

