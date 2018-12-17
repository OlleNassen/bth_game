#include "game.hpp"

int main()
{	
	Game game;
	game.run();
	return 0;
}

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
	Game game;
	game.run();
	return 0;
}