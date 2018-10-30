#ifndef GAMEPLAY_HPP
#define GAMEPLAY_HPP

#include <chrono>
#include <array>
#include <glm/glm.hpp>

#include "lua_script.hpp"
#include "input.hpp"
#include "race.hpp"
#include "../../graphics/include/game_scene.hpp"
#include "../../physics/include/world.hpp"

//struct collision_data
//{
//	glm::vec2 position;
//	float width;
//	float height;
//	bool trigger;
//};

namespace logic
{

class objects
{
public:
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 size;
	glm::vec2 forces;
	glm::vec2 impulse;
};

using objects_array = std::array<objects, 100>;
	   
struct Input
{
	std::chrono::milliseconds delta;
	objects_array& dynamics;
	const input* player_inputs; //4
	std::array<glm::vec3, 4> directions;
	graphics::GameScene* scene;
	physics::World* physics;
};

struct Output
{
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
	int set_player_status(int i, bool status);
	bool get_player_status();
private:
	script_array<int> entities;
	script_array<LuaScript> scripts;
	Race race;

	int model_id{ -1 };
	int physics_id{ -1 };
	int index{ 0 };

	float give_up_timer{ 0.0f };
	void give_up(Input input);
	bool everyone_reached_goal();

	int points = 0;
	std::vector<int> current_gameboard;
};

}

#endif
