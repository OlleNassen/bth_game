#ifndef GAME_HPP
#define GAME_HPP
#include <renderer.hpp>
#include "window.hpp"

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
	Renderer* renderer;
};



#endif