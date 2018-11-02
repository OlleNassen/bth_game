#include "game.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <glm/glm.hpp>

#include <flags.hpp>

using namespace std::chrono_literals;

Game::Game()
	: window(settings.get_window_settings().resolution
	, settings.get_window_settings().fullscreen
	, "Scrap Escape")

	, mesh_lib{0}
	, object_lib{1}
	, level{"../resources/level/level.ssp", &mesh_lib, &object_lib}
	, renderer{&level}
{

	window.assign_key(logic::button::up, GLFW_KEY_W);
	window.assign_key(logic::button::left, GLFW_KEY_A);
	window.assign_key(logic::button::down, GLFW_KEY_S);
	window.assign_key(logic::button::right, GLFW_KEY_D);
	window.assign_key(logic::button::jump, GLFW_KEY_SPACE);
	window.assign_key(logic::button::rotate, GLFW_KEY_R);
	window.assign_key(logic::button::refresh, GLFW_KEY_F5);
	window.assign_key(logic::button::menu, GLFW_KEY_F1);
	window.assign_key(logic::button::debug, GLFW_KEY_F3);
	window.assign_key(logic::button::quit, GLFW_KEY_ESCAPE);

	window.assign_button(logic::button::up, controller_buttons::up);
	window.assign_button(logic::button::left, controller_buttons::left);
	window.assign_button(logic::button::down, controller_buttons::down);
	window.assign_button(logic::button::right, controller_buttons::right);
	window.assign_button(logic::button::jump, controller_buttons::a);
	window.assign_button(logic::button::quit, controller_buttons::b);
	window.assign_button(logic::button::select, controller_buttons::y);

	window.assign_axis_neg(logic::button::left, controller_axis::ls_right);
	window.assign_axis_pos(logic::button::right, controller_axis::ls_right);

	for (int i = 0; i < 12; ++i)
	{
		player_inputs[0][static_cast<logic::button>(i)] = logic::button_state::none;
		player_inputs[1][static_cast<logic::button>(i)] = logic::button_state::none;
		player_inputs[2][static_cast<logic::button>(i)] = logic::button_state::none;
		player_inputs[3][static_cast<logic::button>(i)] = logic::button_state::none;
	}	

	physics.add_dynamic_body(level.v[0], { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });
	physics.add_dynamic_body(level.v[1], { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });
	physics.add_dynamic_body(level.v[2], { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });
	physics.add_dynamic_body(level.v[3], { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });

	for (int i = 0; i < 4; ++i)
	{
		dynamics[i].position = level.v[i];
		dynamics[i].velocity = { 0.0f, 0.0f };
		dynamics[i].size = { 1.0f, 3.5f };
		dynamics[i].forces = { 0.0f, 0.0f };
		dynamics[i].impulse = { 0.0f, 0.0f };
	}

	for (int i = 0; i < 4; ++i)
	{
		std::string name = "P" + std::to_string(i+1);
		player_results[i] = logic::PlayerResult{name, 0.0f};
	}

	for (auto& coll : level.coll_data)
		physics.add_static_body(coll.position, 
			glm::vec2{ 0.0f,0.0f }, coll.width, coll.height, coll.trigger);

	//place_random_objects(0, 20, 9); //For random placing object
}

void Game::run()
{
	using clock = std::chrono::steady_clock;
	auto last_time = clock::now();
	auto delta_time = 0ns;
	auto frames = 0;

	while (window.is_open() && !menu.exit())
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
	std::vector<glm::vec3> db_coll = physics.get_all_debug();
	
	renderer.render(chat.begin(), chat.end(),
		menu.button_strings(),
		db_coll, logic_out.game_over);
}

void Game::update(std::chrono::milliseconds delta)
{
	using std::cout;
	constexpr char nl = '\n';

	int game_state = 0;	

	if (menu.on())
		game_state = (game_state | state::menu);

	if (chat.is_on())
		game_state = (game_state | state::chat);

	if (net.connected())
		game_state = (game_state | state::connected);

	if (gameplay.build_stage())
	{
		game_state = (game_state | state::building);
	}
	else
	{
		game_state = (game_state | state::playing);
	}
		
	

	if ((*local_input)[logic::button::quit] == logic::button_state::pressed)
	{
		menu.open();			
	}

	if (menu.on())
	{
		window.show_cursor();		
	}
	else
	{
		window.hide_cursor();
	}	

	if ((*local_input)[logic::button::debug] == logic::button_state::held)
		game_state = (game_state | state::render_physics);


	chat.update(delta);
	menu.update(delta, *local_input);
		
	const char* str = nullptr;
	if (!chat[1].empty() && !is_client)
	{
		str = chat[1].c_str();
		is_client = true;
	}

	std::array<glm::vec3, 4> directions
	{ 
		glm::vec3{0.0f}, 
		glm::vec3{0.0f},
		glm::vec3{0.0f}, 
		glm::vec3{0.0f} 
	};

	//Giving players building blocks and moving them.
	if (game_state & state::building)
	{
		if (!give_players_objects)
		{
			players_placed_objects_id.fill({ 0, 0 });
			for (int i = 0; i < 4; i++)
			{
				collision_data data;
				int model_id = level.add_object(data, 6);
				int dynamic_id = physics.add_dynamic_body(glm::vec2{ 0, 16 + i }, { 0, 0 }, data.width, data.height, { 0, 0 });

				players_placed_objects_id[i].model_id = model_id;
				players_placed_objects_id[i].dynamics_id = dynamic_id;

				dynamics[dynamic_id].position = { 0, 16 + i };
				dynamics[dynamic_id].velocity = { 0.0f, 0.0f };
				dynamics[dynamic_id].size = { data.width, data.height };
				dynamics[dynamic_id].forces = { 0.0f, 0.0f };
				dynamics[dynamic_id].impulse = { 0.0f, 0.0f };

				give_players_objects = true;
			}
		}

		for (auto& ppoi : players_placed_objects_id)
		{
			level.models[ppoi.model_id].set_position(dynamics[ppoi.dynamics_id].position);
		}
	}
	else
	{
		give_players_objects = false;
	}	

	{
		logic::objects_array obj;
		for (auto i = 0u; i < dynamics.size(); ++i)
		{
			obj[i].position = dynamics[i].position;
			obj[i].velocity = dynamics[i].velocity;
			obj[i].size = dynamics[i].size;
			obj[i].forces = dynamics[i].forces;
			obj[i].impulse = dynamics[i].impulse;
		}
		
		logic_out = gameplay.update(
			{ delta, obj, triggers,
			player_inputs, 
			players_placed_objects_id },
			player_results, game_state);

		for (auto i = 0u; i < dynamics.size(); ++i)
		{
			dynamics[i].position = obj[i].position;
			dynamics[i].velocity = obj[i].velocity;
			dynamics[i].size = obj[i].size;
			dynamics[i].forces = obj[i].forces;
			dynamics[i].impulse = obj[i].impulse;
		}
	}

	if (net.connected())
	{
		for (int i = 0; i < 4; ++i)
		{
			/*
			if (level.models[i].is_animated)
				level.models[i].update_animation((float)delta.count());

			if (player_inputs[i][logic::button::jump] == logic::button_state::held)
			{

				if (level.models[i].get_state() != MODEL_STATE::START_JUMP && level.models[i].get_state() != IN_JUMP && level.models[i].get_state() != FALLING && level.models[i].get_state() != LANDING)
					level.models[i].switch_animation(MODEL_STATE::START_JUMP, 0.005f);

				if (level.models[i].get_animation_done(MODEL_STATE::START_JUMP))
					level.models[i].switch_animation(MODEL_STATE::IN_JUMP, 0.02f);

				if (level.models[i].get_state() == MODEL_STATE::IN_JUMP)
				{
					physics.dynamic_rigidbodies[i].add_force(glm::vec2{ 0.0f, 10.0f });

				}
			}
			else if (level.models[i].get_state() == IN_JUMP && physics.dynamic_rigidbodies[i].get_force().y < -0.1f)
				level.models[i].switch_animation(MODEL_STATE::FALLING, 0.32f);
			else if (glm::abs(physics.dynamic_rigidbodies[i].get_force().y) < 0.1f && level.models[i].get_state() == MODEL_STATE::FALLING)
			{
				level.models[i].switch_animation(MODEL_STATE::LANDING, 0.05f);
			}

			if (glm::abs(dynamics[i].forces.x) > 3.0f && level.models[i].get_state() == MODEL_STATE::IDLE)
				level.models[i].switch_animation(RUNNING, 0.2);
			else if (glm::abs(dynamics[i].forces.x) < 3.0f && level.models[i].get_state() == MODEL_STATE::RUNNING 
				&& level.models[i].get_state() != MODEL_STATE::IDLE && level.models[i].get_state() != MODEL_STATE::TURN)
				level.models[i].switch_animation(IDLE, 0.2);		

			//===================================Turning===================================

			if (logic_out.directions[i].x > 0.0f || player_inputs[i][logic::button::right] == logic::button_state::held)
			{
				if (level.models[i].get_state() != TURN && level.models[i].is_turned_left == true && level.models[i].is_turned_right == false && level.models[i].get_state() == IDLE
					|| level.models[i].get_state() != TURN && level.models[i].is_turned_left == true && level.models[i].is_turned_right == false && level.models[i].get_state() == RUNNING
					|| level.models[i].get_state() == FALLING || level.models[i].get_state() == IN_JUMP || level.models[i].get_state() == START_JUMP)
				{
					if (level.models[i].get_state() != FALLING && level.models[i].get_state() != IN_JUMP && level.models[i].get_state() != START_JUMP)
						level.models[i].switch_animation(TURN, 0.1);

					level.models[i].is_turned_right = true;
					level.models[i].is_turned_left = false;
				}

			}
			else if (logic_out.directions[i].x < 0.0f || player_inputs[i][logic::button::left] == logic::button_state::held)
			{

				if (level.models[i].get_state() != TURN && level.models[i].is_turned_right == true && level.models[i].is_turned_left == false && level.models[i].get_state() == IDLE
					|| level.models[i].get_state() != TURN && level.models[i].is_turned_right == true && level.models[i].is_turned_left == false && level.models[i].get_state() == RUNNING
					|| level.models[i].get_state() == FALLING || level.models[i].get_state() == IN_JUMP || level.models[i].get_state() == START_JUMP)
				{
					if (level.models[i].get_state() != FALLING && level.models[i].get_state() != IN_JUMP && level.models[i].get_state() != START_JUMP)
						level.models[i].switch_animation(TURN, 0.1);

					level.models[i].is_turned_right = false;
					level.models[i].is_turned_left = true;
				}
				//===================================Turning===================================
			}
			if (!level.models[i].is_turned_left && level.models[i].is_turned_right && level.models[i].get_state() != TURN)
			{
				level.models[i].rotate({ 0.0f, 1.0f, 0.0f }, glm::radians(180.0f));
			}
			else if (!level.models[i].is_turned_right && level.models[i].is_turned_left && level.models[i].get_state() != TURN)
			{
				level.models[i].rotate({ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f));
			}
			*/
			glm::vec2 pos
			{
				dynamics[i].position.x,
				dynamics[i].position.y - dynamics[i].size.y - 0.5f
			};
			level.v[i] = pos;
			level.models[i].set_position(pos);

		}
	}




	physics.update(delta, dynamics, triggers);

	pack_data();
	net.update(net_state, str);
	unpack_data();

	{
		graphics::objects_array obj;
		for (auto i = 0u; i < dynamics.size(); ++i)
		{
			obj[i].position = dynamics[i].position;
			obj[i].size = dynamics[i].size;
		}

		std::array<glm::vec3, 4> directions;
		for (int i = 0; i < 4; ++i)
		{
			const auto& in = player_inputs[i];
			auto& direction = directions[i];
			direction = { 0.0f, 0.0f, 0.0f };
			using namespace logic;

			if (in[button::up] >= button_state::pressed)
				direction.z += 1.0f;
			if (in[button::left] >= button_state::pressed)
				direction.x -= 1.0f;
			if (in[button::down] >= button_state::pressed)
				direction.z -= 1.0f;
			if (in[button::right] >= button_state::pressed)
				direction.x += 1.0f;
		}
		
		using namespace std;
		stringstream stream;
		for (auto& p : player_results)	
			stream << p.name << ": "
			<< fixed << setprecision(2) << p.score << " | ";
		
		string temp = stream.str();
		renderer.update(delta,
			obj,
			player_inputs[net.id()].cursor,
			directions,
			chat[1], player_count,
			net.id(), game_state, temp);
	}
}

void Game::pack_data()
{	
	for (int i = 0; i < 4; ++i)
	{
		net_state.inputs[i] = static_cast<logic::uint16>(player_inputs[i]);
	}

	for (auto i = 0u; i < dynamics.size(); ++i)
	{
		net_state.game_objects[i].position = dynamics[i].position;
		net_state.game_objects[i].velocity = dynamics[i].velocity;
	}
}

void Game::unpack_data()
{	
	for (int i = 0; i < 4; ++i)
	{
		if (i != net.id())
		{
			player_inputs[i] = logic::input{net_state.inputs[i]};
		}		
	}
	
	if (state_sequence != net_state.sequence)
	{
		state_sequence = net_state.sequence;
		player_count = net_state.player_count;

		if (net.id())
		{
			for (auto i = 0u; i < dynamics.size(); ++i)
			{
				dynamics[i].position = net_state.game_objects[i].position;
				dynamics[i].velocity = net_state.game_objects[i].velocity;
			}
		}

		local_input = &player_inputs[net.id()];
	}
}

void Game::place_random_objects(float start_height, float map_width, int number_of_randoms)
{
	/*for (int i = 0; i < 15; i++)
	{
		input.physics->random_placed_objects_pos[i] = glm::vec2{ 0.0, hight };
	}*/

	collision_data data;

	glm::vec2 startPosition = { 0.0, 0.0 };
	std::vector<glm::vec2> positions;

	int totalX = 5;
	int totalY = 3;

	int width = static_cast<int>(map_width / 6.0f);

	startPosition = { width * 2, map_width };

	for (int i = 0; i < totalY; i++)
	{
		for (int j = 0; j < totalX; j++)
		{
			positions.push_back({ startPosition.x - (j * width),  startPosition.y + (i * 8) });
		}
	}

	std::vector<int> rand_numb;

	bool same_number = false;
	int randum_number;

	while (rand_numb.size() < number_of_randoms)
	{
		same_number = false;
		randum_number = rand() % positions.size();
		for (int i = 0; i < rand_numb.size(); i++)
		{
			if (randum_number == rand_numb[i])
			{
				same_number = true;
			}
		}
		if (!same_number)
		{
			rand_numb.push_back(randum_number);
		}
	}

	for (int i = 99; i > 99 - number_of_randoms; i--)
	{
		collision_data data;
		int model_id = level.add_object(data, 6);
		data.position = positions[rand_numb[abs(i - 99)]];
		int dynamic_id = physics.add_dynamic_body(data.position, { 0, 0 }, data.width, data.height, { 0, 0 });

		dynamics[dynamic_id].position = positions[rand_numb[abs(i - 99)]];
		dynamics[dynamic_id].velocity = { 0.0f, 0.0f };
		dynamics[dynamic_id].size = { data.width, data.height };
		dynamics[dynamic_id].forces = { 0.0f, 0.0f };
		dynamics[dynamic_id].impulse = { 0.0f, 0.0f };


		level.models[model_id].set_position(dynamics[dynamic_id].position);

	}

}