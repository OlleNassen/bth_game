#ifndef GAMEPLAY_HPP
#define GAMEPLAY_HPP

#include <chrono>
#include <array>
#include <glm/glm.hpp>

#include "lua_load.hpp"
#include "lua_script.hpp"
#include "../../engine/include/input.hpp"

namespace logic
{

struct Input
{
	std::chrono::milliseconds delta;
	const input* local_input;
	std::array<glm::vec3, 4> directions;
};

struct Output
{
	glm::vec2 updated_player_pos;
	std::array<glm::vec2, 4> velocities;
};

template <typename T>
using script_array = std::array<T, 10>;

class Gameplay
{
public:
	Gameplay();

	void refresh();

	Output update(Input input);

private:
	script_array<int> entities;
	script_array<LuaScript> scripts;

	LuaLoad luaLoad; //Test
};

}

#endif
