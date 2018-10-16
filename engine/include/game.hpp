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
	
	Window window; //PUT BELOW WINDOW IF OPENGL RELATED
	
	graphics::MeshLib mesh_lib;
	graphics::GameScene level;
	graphics::Renderer renderer;

	logic::Output logic_out;
	logic::Gameplay gameplay;
	//logic::input player_input;
	logic::input player_inputs[4];
	logic::input* local_input{ &player_inputs[0] };
	
	network::uint32 state_sequence = 0;
	network::GameState net_state;
	network::Messenger net;

	physics::World physics;	
	std::chrono::milliseconds timestep{16};
	
	gui::Chat chat;
	gui::Menu menu;	
};




#endif