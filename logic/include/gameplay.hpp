#ifndef GAMEPLAY_HPP
#define GAMEPLAY_HPP

#include <chrono>
#include <array>
#include <glm/glm.hpp>

#include "lua_load.hpp"
#include "lua_script.hpp"
#include "input.hpp"
#include "../../graphics/include/game_scene.hpp"

namespace logic
{

struct Input
{
	int player_id;
	std::chrono::milliseconds delta;
	const input* local_input;
	std::array<glm::vec3, 4> directions;
	graphics::GameScene* scene;
};

struct Output
{
	glm::vec2 updated_player_pos;
	std::array<glm::vec2, 4> velocities;
	std::array<glm::vec3, 4> directions;
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

	int object_placed_id{ -1 };

	float give_up_timer{ 0.0f };
	void give_up(Input input);
};

}

#endif
