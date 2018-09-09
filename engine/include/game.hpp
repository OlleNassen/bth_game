#ifndef GAME_HPP
#define GAME_HPP
#include <array>
#include <vector>
#include <renderer.hpp>
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
	void update();
	Window window;
	input player_input;
	Renderer* renderer;

	std::vector<Shader> shaders;
};



#endif