#include "game.hpp"
#include <iostream>

using namespace std::chrono_literals;

Game::Game()
	:window(glm::ivec2(1280, 720), "Untitled")
{
	renderer = new Renderer;

	player_input.assign_key(button::up, GLFW_KEY_W);
	player_input.assign_key(button::left, GLFW_KEY_A);
	player_input.assign_key(button::down, GLFW_KEY_S);
	player_input.assign_key(button::right, GLFW_KEY_D);
	player_input.assign_key(button::quit, GLFW_KEY_ESCAPE);
	window.bind(player_input);

	net_init();

	std::string s;
	//Edvards networking code:
	
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
		player_input.state(button::quit) != button_state::pressed)
	{
		delta_time += clock::now() - last_time;
		last_time = clock::now();

		if (delta_time > timestep)
		{
			delta_time = 0ns;
			update(timestep);
		}

		render();
		window.swap_buffers();
		window.poll_events();
	}
}

void Game::render()
{
	renderer->render();			
}

void Game::update(std::chrono::milliseconds delta)
{
	using std::cout;
	constexpr char nl = '\n';

	packet p;
	
	if (player_input.state(button::up) == button_state::pressed)
	{
		p.s = "up";
	}
	if (player_input.state(button::left) == button_state::pressed)
	{
		p.s = "left";
	}
	if (player_input.state(button::down) == button_state::pressed)
	{
		p.s = "down";
	}
	if (player_input.state(button::right) == button_state::pressed)
	{
		p.s = "right";
	}

	renderer->update(delta, player_input);
	//host->update(p);
}
