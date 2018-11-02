#ifndef GAMEPLAY_HPP
#define GAMEPLAY_HPP

#include <chrono>
#include <array>
#include <glm/glm.hpp>
#include <flags.hpp>

#include "lua_script.hpp"
#include "input.hpp"
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

using objects_array = std::array<objects, 100>;
using trigger_array = std::array<int, 100>;
	   
struct Input
{
	std::chrono::milliseconds delta;
	objects_array& dynamics;
	const trigger_array& triggers;
	const input* player_inputs; //4
};

struct Output
{
	bool game_over;
};



template <typename T>
using script_array = std::array<T, 10>;

class Gameplay
{
public:
	Gameplay();

	void refresh();

	Output update(Input input,
		std::array<logic::PlayerResult, 4>& player_results,
		int current_state);
private:
	script_array<int> entities;
	PlayerScript player_script{"../resources/scripts/player.lua"};
	PlayerScript placement_script{"../resources/scripts/placing_objects.lua"};
	GameScript game_script;
	std::array<glm::vec3, 4> directions;

	int model_id{ -1 };
	int physics_id{ -1 };
	int index{ 0 };

	float give_up_timer{ 0.0f };
	void give_up(Input input);

	int points = 0;
	std::vector<int> current_gameboard;
	int	get_random_object_id(Input input);
};

}

#endif
