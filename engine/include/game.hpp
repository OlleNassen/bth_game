#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <array>
#include <vector>
#include <chrono>

#include <renderer.hpp>
#include <network.hpp>
#include <gameplay.hpp>
#include <world.hpp>
#include <input.hpp>

#include <algorithm>

#include "mesh_lib.hpp"
#include "game_scene.hpp"
#include "window.hpp"

#include "settings.hpp"
#include "gui.hpp"
#include "timer.hpp"

//::.. authors ..:://
// Edvard
// Olle

class Game
{
public:
	Game();
	void run();

private:
	void render();
	void update(std::chrono::milliseconds delta);

	void pack_data();
	void unpack_data();
	
	Settings settings; // NEEDS TO BE ABOVE WINDOW
	Window window; //PUT BELOW WINDOW IF OPENGL RELATED
	
	graphics::MeshLib mesh_lib;
	graphics::MeshLib object_lib;
	
	int level_id = 0;
	graphics::GameScene level1{"../resources/level/level_1.ssp", &mesh_lib, &object_lib}; //ID = 1
	graphics::GameScene level2{"../resources/level/level_2.ssp", &mesh_lib, &object_lib}; //ID = 2
	graphics::GameScene lobby{"../resources/level/lobby.ssp", &mesh_lib, &object_lib}; //ID = 0

	graphics::GameScene* level{&lobby};

	graphics::Renderer renderer;

	logic::Gameplay gameplay;
	logic::input player_inputs[4];
	logic::input* local_input{ &player_inputs[0] };
	logic::LuaExport lua_data;

	logic::trigger_type_array triggers_types; //test for triggers
	
	network::uint8 player_count = 0;
	network::uint32 state_sequence = 0;
	network::GameState net_state;
	network::Messenger net;

	physics::trigger_array triggers;
	physics::objects_array dynamics;
	physics::World physics;	
	std::chrono::milliseconds timestep{16};

	gui::Chat chat;
	gui::Menu menu;	

	//Build info
	std::vector<build_information> build_info;
	int has_placed_correctly[4] = { 0 };

	void place_random_objects(float start_height, int number_of_randoms);

	std::array<anim, 4> anim_states;

	//Door voting
	int door_1_votes = 0;
	int door_2_votes = 0;

	//Temp leaderboards
	std::vector<int> leader_board;
	bool showleaderboard = false;
	bool is_client{};

	//Building phase
	std::array <id_and_model_place, 4> players_placed_objects_id;
	std::vector<glm::vec3> placed_objects_hitbox_positions;
	bool buildmode = true;
	bool give_players_objects = false;
	std::vector<glm::vec3> remove_lines;

	std::vector<build_information> all_placed_objects;
	int total_nr_objects = 0;
	struct turret_info
	{
		int rotation; 
		int direction;
	};
	std::array<turret_info, 4> turret_infos;

	void remove_object(int id);

	//Turret 
	struct turret
	{
		int dynamic_id;
		glm::vec2 direction;
		glm::vec2 barrel_position; // start
		glm::vec2 end_position;
		float range;
	};
	std::vector<turret> turrets;
	std::array<bool, 4> player_hit_array = { false, false, false, false };
	void laser_update(turret turret, std::array<bool, 4>& hit_array);
	void add_turret(const int dyn_id, const turret_info dir_info, const glm::vec2 turret_pos);

	//Random placed objects
	std::array <id_and_model_place, 4> random_placed_objects_id;

	//moving platforms
	void add_moving_platforms();
	std::vector<int> moving_platform_ids;
	int nr_of_moving_platforms = 0;

	//test trigger
	static const int num_objects = 8;
	int placed_objects_type_id = 7;
	//int placed_objects_array[4] = {7/*random*/, 2/*glide*/, 7/*speed*/, 6/*shield*/ };
	int placed_objects_array[num_objects] = { 0, 1, 2, 3, 4, 5, 6, 7 };

	int random_picked_object() { return placed_objects_array[rand() % num_objects]; };
	std::array<int, 4> random_indexes();

	//spectator
	int watching = 0;
	int spikeframe = 0;
	int turretframe = 0;

	int find_next_spectator(int current_id);
	int find_previous_spectator(int current_id);

	//New gameloop stuff
	void load_map(graphics::GameScene* scene);

	//Random Seed
	std::array<int, 4> buff_activ = { -1, -1, -1, -1 };
	std::array<int, 5> random_values = { -1 };
};




#endif