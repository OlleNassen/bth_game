#ifndef GAME_HPP
#define GAME_HPP
#include "window.hpp"

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
};



#endif