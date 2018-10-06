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
	n();
	~n();
	
	output update(input input);

private:
	std::unique_ptr<Host> host;
};

}
#endif

