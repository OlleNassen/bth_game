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
	window.assign_key(button::quit, GLFW_KEY_ESCAPE);

	net_init();

	std::cin >> s;

	if (s == "server")
	{
		host = std::make_unique<Server>();
	}
	else
	{
		host = std::make_unique<Client>(s);
	}
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
		player_inputs.components[host->id()][button::quit] != button_state::pressed)
	{
		delta_time += clock::now() - last_time;
		last_time = clock::now();

		if (delta_time > timestep)
		{
			delta_time = 0ns;
			window.update_input(player_inputs.components[host->id()]);
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

	Packet p;
	p.i = player_inputs.components;
	
	host->update(p, 
		std::begin(player_inputs.components), 
		std::end(player_inputs.components));
	
	renderer.update(delta, player_inputs.components[0], 0);
	renderer.update(delta, player_inputs.components[1], 1);
	renderer.update(delta, player_inputs.components[2], 2);
	renderer.update(delta, player_inputs.components[3], 3);
}
