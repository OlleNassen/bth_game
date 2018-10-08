#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <memory>
#include <string>
#include <array>
#include <glm/glm.hpp>

#include "host.hpp"

namespace network
{

struct Input
{
	const std::string& chat;
	const std::array<glm::vec3, 4>& directions;
};

struct Output
{
	int player_id;
	int player_count;
	std::array<glm::vec3, 4> directions;
};

class Messenger
{
public:
	Messenger();
	~Messenger();

	bool connected() const { return static_cast<bool>(host); }
	
	Output update(Input input);

private:
	std::unique_ptr<Host> host;
};

}
#endif

