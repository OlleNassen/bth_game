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
	graphics::GameScene level;
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

	void place_random_objects(float start_height, float map_width, int number_of_randoms);

	std::array<anim, 4> anim_states;

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

	void remove_object(int id);

	//Random placed objects
	std::array <id_and_model_place, 4> random_placed_objects_id;

	//test trigger
	int placed_objects_list_id = 7;
	int placed_objects_array[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	int random_picked_object() { return placed_objects_array[rand() % 10]; };
	std::array<int, 4> random_indexes();

	//spectator
	int watching = 0;
	bool changed_player = false;

	//New gameloop stuff
	void load_map(const char* file_path);
};




#endif