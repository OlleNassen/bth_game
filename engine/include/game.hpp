#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <array>
#include <vector>
#include <chrono>

#include <renderer.hpp>
#include <host.hpp>
#include "ecs.hpp"
#include "window.hpp"
#include "input.hpp"
#include "gui.hpp"

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
	~Game();
	void run();
private:
	void render();
	void update(std::chrono::milliseconds delta);

	std::string s;
	
	std::unique_ptr<Host> host;
	std::chrono::milliseconds timestep{16};
	Window window;
	input player_input;
	Renderer renderer;
	input_array<8> player_inputs;
	GraphicalUserInterface gui;
	
};



#endif