#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <array>
#include <vector>
#include <chrono>

#include <renderer.hpp>
#include <host.hpp>
#include "window.hpp"
#include "input.hpp"

//::.. authors ..:://
// Edvard
// Olle

class Game
{
public:
	Game();
	~Game();
	void run();
private:
	void render();
	void update(std::chrono::milliseconds delta);
	
	std::unique_ptr<host> host;
	std::chrono::milliseconds timestep{16};
	Window window;
	input player_input;
	Renderer* renderer;

	std::vector<Shader> shaders;
};



#endif