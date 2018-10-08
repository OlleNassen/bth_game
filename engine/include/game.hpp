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

#include "ecs.hpp"
#include "mesh_lib.hpp"
#include "game_scene.hpp"
#include "window.hpp"
#include "input.hpp"

#include "gui.hpp"
#include "timer.hpp"

//::.. authors ..:://
// Edvard
// Olle

template <int N>
struct input_array
{
	int player_index;
	std::array<ecs::entity, N> entities;
	input components[N];
};

class Game
{
public:
	Game();
	void run();

private:
	void render(std::vector<glm::vec2> debug_positions);
	void update(std::chrono::milliseconds delta);
	
	Window window; //PUT BELOW WINDOW IF OPENGL RELATED
	
	graphics::MeshLib mesh_lib;
	graphics::GameScene level;
	graphics::Renderer renderer;

	logic::Output logic_out;
	logic::Gameplay gameplay;
	
	network::Output net_out;
	network::Messenger net;

	physics::World physics;
	
	input player_input;
	std::chrono::milliseconds timestep{16};
	
	input_array<4> player_inputs;
	input* local_input{ &player_inputs.components[0] };
	gui::Chat chat;
	gui::Menu menu;	
};




#endif