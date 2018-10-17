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
	window.assign_key(logic::button::debug, GLFW_KEY_R);
	window.assign_key(logic::button::switch_object, GLFW_KEY_F4);
	window.assign_key(logic::button::remove_object, GLFW_KEY_O);
	window.assign_key(logic::button::rotate, GLFW_KEY_F3);	
	window.assign_key(logic::button::build_mode, GLFW_KEY_B);
	window.assign_key(logic::button::place_object, GLFW_KEY_KP_ENTER);
	window.assign_key(logic::button::quit, GLFW_KEY_ESCAPE);

	logic_out.directions.fill({ 0.0f, 0.0f, 0.0f });

	physics.add_dynamic_body(level.v[0], { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });
	physics.add_dynamic_body(level.v[1], { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });
	physics.add_dynamic_body(level.v[2], { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });
	physics.add_dynamic_body(level.v[3], { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });

	for (auto& coll : level.coll_data)
		physics.add_static_body(coll.position, 
			glm::vec2{ 0.0f,0.0f }, coll.width, coll.height, coll.trigger);
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
		net.connected(), menu.debug());
}

void Game::update(std::chrono::milliseconds delta)
{
	using std::cout;
	constexpr char nl = '\n';

	/*if ((*local_input)[logic::button::build_mode] == logic::button_state::pressed)
	{
		level.build_mode_active = !level.build_mode_active;
	}*/

	/*auto& direction = net_out.directions[net_out.player_id];
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
	}*/


	/*if (level.build_mode_active && (*local_input)[logic::button::place_object] == logic::button_state::pressed)
	{
		level.add_object(glm::vec2(15.0f, 0.0f));
	}*/


	if ((*local_input)[logic::button::debug] == logic::button_state::pressed)
	{
		renderer.debug_active = !renderer.debug_active;
	}

	if (!menu.on())
		window.hide_cursor();

	std::vector<glm::vec2> forces = physics.get_forces();

	const char* str = nullptr;
	static bool connected;
	if (!chat[1].empty() && !connected)
	{
		str = chat[1].c_str();
		connected = true;
	}

	pack_data();
	net.update(net_state, str);
	unpack_data();

	chat.update(delta);
	menu.update(delta, *local_input);

	std::array<glm::vec3, 4> directions
	{ 
		glm::vec3{0.0f}, 
		glm::vec3{0.0f},
		glm::vec3{0.0f}, 
		glm::vec3{0.0f} 
	};
	
	logic_out = gameplay.update({ net.id(), delta, local_input, directions, &level, &physics });
	glm::vec2 updated_player_pos = logic_out.updated_player_pos;
	
	physics.update(delta);

	if ((*local_input)[logic::button::jump] == logic::button_state::pressed && net.connected())
		physics.dynamic_rigidbodies[net.id()].add_force(glm::vec2{ 0.0f, 50.0f });

	for (int i = 0; i < 4; ++i)
	{	
		if (net.connected())
			physics.dynamic_rigidbodies[i].add_force(logic_out.velocities[i]);
		level.v[i] = physics.dynamic_positions[i];
		level.models[i].set_position(physics.dynamic_positions[i]);
	}		

	renderer.update(delta,
		player_inputs[net.id()].cursor,
		logic_out.directions,
		chat[1], net.connected_players(),
		net.id(), chat.is_on(),
		net.connected());

}

void Game::pack_data()
{
	network::uint64 input_int = 0;
	for (const auto& in : player_inputs)
		input_int = (input_int | static_cast<logic::uint16>(in));
	net_state.input = input_int;

	for (int i = 0; i < physics.dynamic_positions.size(); ++i)
	{
		net_state.game_objects[i].position = physics.dynamic_positions[i];
	}
}

void Game::unpack_data()
{
	if (state_sequence != net_state.sequence)
	{
		state_sequence = net_state.sequence;
	}	
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			using logic::uint16;
			uint16 in = static_cast<uint16>(net_state.input << 16);
			if (in && i != net.id())
				player_inputs[i] = logic::input{ in };
		}

		for (int i = 0; i < physics.dynamic_positions.size(); ++i)
		{
			if (net_state.game_objects[i].position.x != 0 || net_state.game_objects[i].position.y != 0)
				physics.dynamic_positions[i] = net_state.game_objects[i].position;
		}

		local_input = &player_inputs[net.id()];
	}
}
