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
		(*local_input)[button::quit] != button_state::pressed)
	{
		delta_time += clock::now() - last_time;
		last_time = clock::now();

		if (delta_time > timestep)
		{
			delta_time = 0ns;
			window.update_input(*local_input);
			update(timestep);
		}

		render();
		window.swap_buffers();
		window.poll_events();		
	}
}

void Game::render()
{
	renderer.render(chat.begin(), chat.end());
}

void Game::update(std::chrono::milliseconds delta)
{
	using std::cout;
	constexpr char nl = '\n';

	if (!host && !chat[1].empty())
	{
		if (chat[1] == "server")
		{
			host = std::make_unique<Server>();
			
		}
		else
		{
			host = std::make_unique<Client>(s);
		}
		local_input = &player_inputs.components[host->id()];
	}
	else if (host)
	{
		Packet p;
		p.i = player_inputs.components;
		host->update(p, 
			std::begin(player_inputs.components), 
			std::end(player_inputs.components));
	}

	renderer.update(delta, player_inputs.components[0], 0, true);
	//renderer.update(delta, player_inputs.components[1], 1, true);
	//renderer.update(delta, player_inputs.components[2], 2, true);
	//renderer.update(delta, player_inputs.components[3], 3, true);
	chat.update(delta);
}
