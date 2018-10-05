#include "game.hpp"
#include <iostream>

using namespace std::chrono_literals;

Game::Game()
	: window({1280, 720}, "Scrap Escape")
{
	window.assign_key(button::up, GLFW_KEY_W);
	window.assign_key(button::left, GLFW_KEY_A);
	window.assign_key(button::down, GLFW_KEY_S);
	window.assign_key(button::right, GLFW_KEY_D);
	window.assign_key(button::jump, GLFW_KEY_SPACE);
	window.assign_key(button::glow, GLFW_KEY_G);
	window.assign_key(button::refresh, GLFW_KEY_F5);
	window.assign_key(button::menu, GLFW_KEY_F1);
	window.assign_key(button::debug, GLFW_KEY_F3);
	window.assign_key(button::reset, GLFW_KEY_R);
	window.assign_key(button::quit, GLFW_KEY_ESCAPE);

	mesh_lib = new MeshLib();

	level = new GameScene("../resources/level/level.ssp", mesh_lib);

	renderer = new Renderer(level);

	net_init();
}

Game::~Game()
{
	net_deinit();
	//delete mesh_lib;
	delete level;
	delete renderer;
}

void Game::run()
{
	using clock = std::chrono::steady_clock;
	auto last_time = clock::now();
	auto delta_time = 0ns;

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

		render();
		window.swap_buffers();
		window.poll_events();		
	}
}

void Game::render()
{
	renderer->render(chat.begin(), chat.end(), 
		menu.button_data(), 
		menu.on(),
		static_cast<bool>(host), menu.debug());
}

void Game::update(std::chrono::milliseconds delta)
{
	using std::cout;
	constexpr char nl = '\n';

	player_data net_data;
	std::fill(
		std::begin(net_data.directions),
		std::end(net_data.directions),
		glm::vec3{0.0f});

	auto index = 0;
	std::for_each(
		std::begin(player_inputs.components), 
		std::end(player_inputs.components), [&](auto& i) 
	{
		auto& direction = net_data.directions[index++];

		if (i[button::up] >= button_state::pressed)
			direction.z += 1.0f;
		if (i[button::left] >= button_state::pressed)
			direction.x -= 1.0f;
		if (i[button::down] >= button_state::pressed)
			direction.z -= 1.0f;
		if (i[button::right] >= button_state::pressed)
			direction.x += 1.0f;
	});

	if (!menu.on()) window.hide_cursor();

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
		host->update(&net_data);
	}
	chat.update(delta);
	menu.update(delta, *local_input);

	gameplay.update(delta);

	//check if player/players reached goal

	//Player control-input
	glm::vec2 updated_player_pos = luaLoad.process_input(*local_input, delta);

	renderer->update(delta,
		std::begin(player_inputs.components),
		std::end(player_inputs.components),
		std::begin(net_data.directions),
		std::end(net_data.directions), 
		chat[1], net_data.player_count, 
		net_data.player_id, chat.is_on(), 
		static_cast<bool>(host));

}
