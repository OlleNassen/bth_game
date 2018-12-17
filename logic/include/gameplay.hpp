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

struct id_and_model_place
{
	int dynamics_id = -1;
	int model_id = -1;
	int place_state = 1;
	int model_type_id = -1;

	bool operator==(const id_and_model_place& other) 
	{
		return	dynamics_id		== other.dynamics_id &&
				model_id		== other.model_id &&
				place_state		== other.place_state &&
				model_type_id	== other.model_type_id;
	}

	bool operator!=(const id_and_model_place& other)
	{
		return	dynamics_id != other.dynamics_id ||
			model_id != other.model_id ||
			place_state != other.place_state ||
			model_type_id != other.model_type_id;
	}
};

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
	std::array<anim, 4>& anim_states;
	std::array<id_and_model_place, 4>& players_placed_objects_id;
	int player_count = 0;
	int spike_keyframe = 0;
	int turret_keyframe = 0;

	const trigger_type_array& triggers_types; //test for triggers
	std::array<bool, 4> laser_hit_array;

	std::array<int, 5> random_values;
	std::array<int, 4>& buff_activ;
	const std::vector<int> moving_platform_ids;
};


template <typename T>
using script_array = std::array<T, 10>;

class Gameplay
{
public:
	Gameplay();

	void refresh();

	LuaExport update(const Input& inputs,
		int& current_state, bool rw[], bool lw[], int player_id);
	bool build_stage(int player_count) const;
	bool pre_playing_stage() const;
	bool is_new_round = false;
	void new_round();
private:
	script_array<int> entities;
	PlayerScript player_script{"../resources/scripts/player.lua"};
	PlacingScript placement_script{"../resources/scripts/placing_objects.lua"};
	GameScript game_script;
	MovingPlatformsScript moving_platforms_script{ "../resources/scripts/moving_platforms.lua" };

	std::array<glm::vec3, 4> directions;

	int model_id{ -1 };
	int physics_id{ -1 };
	int index{ 0 };

	float give_up_timer{ 0.0f };
	void give_up(const Input& input);

	int points = 0;
	std::vector<int> current_gameboard;
	int	get_random_object_id(const Input& input);
	int players_done = 0;
	bool pre_playing_done = false;
	float pre_starter_time = 3.5f;
};

}

#endif
