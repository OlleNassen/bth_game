#include "game.hpp"
#include <host.hpp>
#include <crtdbg.h>

int main()
{
	//test_net();
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	Game game;
	game.run();
	return 0;
}