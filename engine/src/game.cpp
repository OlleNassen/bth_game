#include "game.hpp"
#include <iostream>

using namespace std::chrono_literals;

Game::Game()
	: window({1280, 720}, "Scrap Escape")
	, level{"../resources/level/level.ssp", &mesh_lib}
	, renderer{&level}
{
	window.assign_key(logic::button::up, GLFW_KEY_W);
	window.assign_key(logic::button::left, GLFW_KEY_A);
	window.assign_key(logic::button::down, GLFW_KEY_S);
	window.assign_key(logic::button::right, GLFW_KEY_D);
	window.assign_key(logic::button::jump, GLFW_KEY_SPACE);
	window.assign_key(logic::button::glow, GLFW_KEY_G);
	window.assign_key(logic::button::refresh, GLFW_KEY_F5);
	window.assign_key(logic::button::menu, GLFW_KEY_F1);
	window.assign_key(logic::button::debug, GLFW_KEY_F3);
	window.assign_key(logic::button::switch_camera, GLFW_KEY_F4);
	window.assign_key(logic::button::give_up, GLFW_KEY_O);
	window.assign_key(logic::button::reset, GLFW_KEY_R);	
	window.assign_key(logic::button::build_mode, GLFW_KEY_B);
	window.assign_key(logic::button::place_object, GLFW_KEY_KP_ENTER);
	window.assign_key(logic::button::quit, GLFW_KEY_ESCAPE);


	net_out.player_id = 0;
	net_out.player_count = 1;
	net_out.directions.fill({ 0.0f, 0.0f, 0.0f });
	//logic_out.directions.fill({ 0.0f, 0.0f, 0.0f });

	physics.add_dynamic_body(level.v[0], { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });
	physics.add_dynamic_body(level.v[1], { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });
	physics.add_dynamic_body(level.v[2], { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });
	physics.add_dynamic_body(level.v[3], { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });

	for (auto& coll : level.coll_data)
		physics.add_static_body(coll.position, 
			glm::vec2{ 0.0f,0.0f }, coll.width, coll.height, coll.trigger);


	//A Temporary goal-object
	physics.add_static_body({7.0f, 11.0f}, glm::vec2{ 0.0f, 0.0f }, 2, 2, true);
	
	//Temporary leaderboard in the game4
	leader_board.resize(4);
	
}

void Game::run()
{
	using clock = std::chrono::steady_clock;
	auto last_time = clock::now();
	auto delta_time = 0ns;
	auto frames = 0;
	std::chrono::duration<float> seconds = 0s;

	while (window.is_open() && 
		!menu.exit() &&
		(*local_input)[logic::button::quit] != logic::button_state::pressed)
	{
		delta_time += clock::now() - last_time;
		last_time = clock::now();

		seconds += delta_time;
		auto fps = static_cast<int>(++frames / seconds.count());
		if (seconds > 1s)
		{
			seconds = 0s;
			frames = 0;
			std::string title = 
				"Scrap Escape | FPS: " 
				+ std::to_string(fps);
			
			window.title(title.c_str());
		}
		
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
	std::vector<glm::vec2> db_coll = physics.get_all_debug();
	
	renderer.render(chat.begin(), chat.end(),
		menu.button_strings(),
		db_coll, menu.on(),
		net.connected(), menu.debug(), leader_board, showleaderboard);
}

void Game::update(std::chrono::milliseconds delta)
{
	using std::cout;
	constexpr char nl = '\n';

	//net_out.directions = logic_out.directions;
	/*if ((*local_input)[logic::button::build_mode] == logic::button_state::pressed)
	{
		level.build_mode_active = !level.build_mode_active;
	}*/

	auto& direction = net_out.directions[net_out.player_id];
	direction = { 0.0f, 0.0f, 0.0f };

	if (!level.build_mode_active)
	{
		if ((*local_input)[logic::button::up] >= logic::button_state::pressed)
			direction.z += 1.0f;
		if ((*local_input)[logic::button::left] >= logic::button_state::pressed)
			direction.x -= 1.0f;
		if ((*local_input)[logic::button::down] >= logic::button_state::pressed)
			direction.z -= 1.0f;
		if ((*local_input)[logic::button::right] >= logic::button_state::pressed)
			direction.x += 1.0f;
	}

	/*if (level.build_mode_active && (*local_input)[logic::button::place_object] == logic::button_state::pressed)
	{
		level.add_object(glm::vec2(15.0f, 0.0f));
	}*/

	if (!menu.on())
		window.hide_cursor();

	std::vector<glm::vec2> forces = physics.get_forces();

	net_out = net.update({ chat[1], net_out.directions, physics.dynamic_positions, forces });
	local_input = &player_inputs[net_out.player_id];

	//for (auto i = 0; i < 4; ++i)
		//physics.dynamic_positions[i] = net_out.positions[i];

	chat.update(delta);
	menu.update(delta, *local_input);

	logic_out = gameplay.update({ net_out.player_id, delta, local_input, net_out.directions });

	glm::vec2 combined_force = logic_out.force;

	//Update new direction force of player to physics
	if (physics.dynamic_rigidbodies[0].can_jump == true)
	{
		physics.dynamic_rigidbodies[0].add_force(combined_force);
	}
	else
		physics.dynamic_rigidbodies[0].add_force({ combined_force.x, 0 });

	std::vector<glm::vec2> dynamic_pos = physics.update(delta);
	physics.update(delta);

	//Temp test for leaderboard stuff
	for (int i = 0; i < net_out.player_count; i++)
	{
		if (physics.dynamic_rigidbodies[i].get_reached_goal() && gameplay.get_player_status())
		{
			leader_board.at(i) += gameplay.set_player_status(i, false);	//Should change the status on players who reached goal
			
			showleaderboard = true;
			//add show leaderboard here
			//renderer.show_leaderboard();
		}
	}

	for (int i = 0; i < 4; ++i)
	{
		if (net.connected())
			//physics.dynamic_rigidbodies[i].add_force(logic_out.force);
			level.v[i] = physics.dynamic_positions[i];
		level.models[i].set_position(physics.dynamic_positions[i]);
	}

	renderer.update(delta,
		player_inputs[net_out.player_id].cursor,
		net_out.directions,
		chat[1], net_out.player_count,
		net_out.player_id, chat.is_on(),
		net.connected());

	
	/*
	if (physics.intersects(net_out.player_id, physics.static_box_colliders.size() -1))
	{
		
		physics.dynamic_rigidbodies[net_out.player_id].cancel_forces();
		w_time -= delta;

		if (w_time < 0s)
		{
			w_time = { 5000 };
			physics.dynamic_positions[net_out.player_id] = glm::vec2(0.0, 0.0);
		}
	}
	*/

}
