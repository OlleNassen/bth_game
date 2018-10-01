#include "game.hpp"
#include <iostream>

using namespace std::chrono_literals;

Game::Game()
	:window(glm::ivec2(1280, 720), "Scrap Escape")
{
	window.assign_key(button::select, GLFW_KEY_Y);
	window.assign_key(button::up, GLFW_KEY_W);
	window.assign_key(button::left, GLFW_KEY_A);
	window.assign_key(button::down, GLFW_KEY_S);
	window.assign_key(button::right, GLFW_KEY_D);
	window.assign_key(button::glow, GLFW_KEY_G);
	window.assign_key(button::refresh, GLFW_KEY_F5);
	window.assign_key(button::menu, GLFW_KEY_F1);
	window.assign_key(button::debug, GLFW_KEY_F3);
	window.assign_key(button::switch_camera, GLFW_KEY_F4);
	window.assign_key(button::reset, GLFW_KEY_R);
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

	physics.add_dynamic_body({ 10, 10 }, { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });
	physics.add_dynamic_body({ -5.0, -5.0 }, { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });
	physics.add_dynamic_body({ 14.0, 2.0 }, { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });
	physics.add_dynamic_body({ -4.0, -20.0 }, { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });

	physics.add_static_body({ 0.0, -22.0 }, { 0.0, 0.0 }, 1000, 2, false);

	while (window.is_open() && 
		!menu.exit() &&
		(*local_input)[button::quit] != button_state::pressed)
	{
		delta_time += clock::now() - last_time;
		last_time = clock::now();

		while (delta_time > timestep)
		{
			delta_time -= timestep;
			window.update_input(*local_input);
			
			update(timestep);
		}

		std::vector<glm::vec2> debug_positions = physics.get_all_debug();
		render(debug_positions);
		window.swap_buffers();
		window.poll_events();		
	}
}

void Game::render(std::vector<glm::vec2> debug_positions)
{
	renderer.render(chat.begin(), chat.end(), menu.button_data(), debug_positions);
}

void Game::update(std::chrono::milliseconds delta)
{
	using std::cout;
	constexpr char nl = '\n';

	if ((*local_input)[button::menu] == button_state::pressed)
	{
		window.show_cursor();
	}

	if (!host && !chat[1].empty())
	{
		if (chat[1] == "server")
		{
			host = std::make_unique<Server>();
			
		}
		else
		{
			host = std::make_unique<Client>(chat[1]);
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
	chat.update(delta);
	menu.update(delta, *local_input);

	std::vector<glm::vec2> dynamic_pos = physics.update(delta);
	
	renderer.update(delta, 
		std::begin(player_inputs.components),
		std::end(player_inputs.components), chat[1], chat.is_on(),
		dynamic_pos);

}
