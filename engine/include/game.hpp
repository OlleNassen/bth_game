#ifndef GAME_HPP
#define GAME_HPP
#include "window.hpp"

class Game
{
private:
	Window window;
public:
	Game();
	~Game();

	void run();
};

#endif