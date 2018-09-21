#include "game.hpp"
#include <iostream>

using namespace std::chrono_literals;

Game::Game()
	:window(glm::ivec2(1280, 720), "Scrap Escape")
{
	window.assign_key(button::up, GLFW_KEY_W);
	window.assign_key(button::left, GLFW_KEY_A);
	window.assign_key(button::down, GLFW_KEY_S);
	window.assign_key(button::right, GLFW_KEY_D);
	window.assign_key(button::glow, GLFW_KEY_G);
	window.assign_key(button::quit, GLFW_KEY_ESCAPE);

	net_init();

	/*std::cin >> s;

	if (s == "server")
	{
		host = std::make_unique<server>();
	}
	else
	{
		host = std::make_unique<client>(s);
	}*/
	
}

Game::~Game()
{
	net_deinit();
}

void Game::run()
{
	using clock = std::chrono::steady_clock;
	auto last_time = clock::now();
	auto delta_time = 0ns;
	
	while (window.is_open() && 
		player_input[button::quit] != button_state::pressed)
	{
		delta_time += clock::now() - last_time;
		last_time = clock::now();

		if (delta_time > timestep)
		{
			delta_time = 0ns;
			window.update_input(player_input);
			update(timestep);
		}

		render();
		window.swap_buffers();
		window.poll_events();		
	}
}

void Game::render()
{
	renderer.render();			
}

void Game::update(std::chrono::milliseconds delta)
{
	using std::cout;
	constexpr char nl = '\n';

	packet p;
	p.i = &player_input;
	/*host->update(p);

	if (host->i && s == "server")
	{
		input& inp = *host->i;
		player_input.cursor = inp.cursor;
		for (int i = 0; i < 5; ++i)
		{		
			player_input[static_cast<button>(i)] = inp[static_cast<button>(i)];			
		}
		host->i = nullptr;
	}*/

	if (player_input[button::up] == button_state::pressed)
		cout << "funkar";

	renderer.update(delta, player_input);
}
