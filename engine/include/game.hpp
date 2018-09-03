#ifndef GAME_HPP
#define GAME_HPP
#include "placeholder.hpp"
#include "window.hpp"

class Game
{
private:
	Placeholder placeholder;
	Window window;
public:
	Game();
	~Game();

	void run();
};

#endif