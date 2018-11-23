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
	, level{"../resources/level/lobby.ssp", &mesh_lib, &object_lib}
	, renderer{&level}
{
	anim_states[0] = anim::idle;
	anim_states[1] = anim::idle;
	anim_states[2] = anim::idle;
	anim_states[3] = anim::idle;

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
		dynamics[i].shield_active = false; //trigger
		anim_states[i] = anim::idle;
	}

	for (auto& coll : level.coll_data)
		physics.add_static_body(coll.position, 
			glm::vec2{ 0.0f, 0.0f }, coll.width, coll.height, coll.trigger);

	//place_random_objects(0, 20, 9); //For random placing object


	//Start States
	net_state.state = network::SessionState::lobby;
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

	build_info.clear();

	for (int i = 0; i < static_cast<int>(player_count); i++)
	{
		int d_id = players_placed_objects_id[i].dynamics_id;
		build_information info;

		info.local_position = glm::vec3(dynamics[d_id].position.x, dynamics[d_id].position.y, 0.0f);
		info.debug_positions = physics.get_debug_for(d_id);
		info.place_state = players_placed_objects_id[i].place_state;
		info.object_id = players_placed_objects_id[i].model_type_id;

		build_info.push_back(info);
	}

	renderer.render(chat.begin(), chat.end(),
		menu.button_strings(),
		db_coll, build_info, lua_data.game_over, lua_data.died, 
		lua_data.finished, lua_data.scores, lua_data.time,
		net.id(),
		players_placed_objects_id[net.id()].model_type_id,
		remove_lines);
}

void Game::update(std::chrono::milliseconds delta)
{
	using std::cout;
	constexpr char nl = '\n';

	float dt = std::chrono::duration_cast<std::chrono::duration<float>>(delta).count();
	int game_state = 0;

	if (net_state.state == network::SessionState::waiting)
	{
		net_state.state = network::SessionState::lobby;
		game_state = (game_state | state::lobby);

		load_map("../resources/level/lobby.ssp");
	}

	/*if (net_state.state == network::SessionState::waiting)
	{
		net_state.state = network::SessionState::none;
		game_state = (game_state | state::lobby);
		physics.clear_object();
		level.clear_object();
		gameplay.refresh();
		for (int i = 0; i < 4; ++i)
			dynamics[i].position = glm::vec2(3.f * i, 2.5f);
		for (int i = 4; i < dynamics.size(); ++i)
			dynamics[i].position = glm::vec2(-20000.f, -20000.f);
		give_players_objects = false;
		watching = net.id();

		level = graphics::GameScene("../resources/level/lobby.ssp", &mesh_lib, &object_lib);
		physics.clear_static_object();
		for (auto& coll : level.coll_data)
		{
			physics.add_static_body(coll.position,
				glm::vec2{ 0.0f,0.0f }, coll.width, coll.height, coll.trigger);
		}
		gameplay.is_new_round = false;
	}	*/

	/*if (gameplay.is_new_round)
	{
		net_state.state = network::SessionState::building;
		game_state = (game_state | state::building);

		give_players_objects = false;
		watching = net.id();
		gameplay.is_new_round = false;
	}*/

	if (menu.on())
		game_state = (game_state | state::menu);

	if (chat.is_on())
		game_state = (game_state | state::chat);

	if (net.connected())
		game_state = (game_state | state::connected);
	
	

	if ((net_state.state == network::SessionState::lobby))
	{
		game_state = (game_state | state::lobby);

		if ((*local_input)[logic::button::rotate] == logic::button_state::pressed && !(game_state & state::menu))
		{
			if (net.id() == 0)
				net_state.state = network::SessionState::loading;
			game_state = (game_state | state::loading);
		}
	}
	else if (net_state.state == network::SessionState::loading)
	{
		game_state = (game_state | state::loading);

		gameplay.refresh();
		load_map("../resources/level/level_1.ssp");

		/*for (int i = 0; i < 4; ++i)
			dynamics[i].position = glm::vec2(3.f * i, 2.5f);*/

		if (net.id() == 0)
			net_state.state = network::SessionState::pre_building;
		game_state = (game_state | state::pre_building);
	}
	else if (net_state.state == network::SessionState::pre_building)
	{
		//Render text of state and what to do.
		game_state = (game_state | state::pre_building);
		static float pre_build_timer = 3.5f;

		for (int i = 0; i < 4; i++)
		{
			dynamics[i].player_moving_object_id = -1;
			dynamics[i].player_moving_object_type_id = -1;
		}

		//Set State -> building
		pre_build_timer -= dt;
		if (pre_build_timer <= 0.0f)
		{
			pre_build_timer = 3.5f;
			if (net.id() == 0)
				net_state.state = network::SessionState::building;
			game_state = (game_state | state::building);
		}
	}
	else if (net_state.state == network::SessionState::building)
	{
		//Inititate player objects and move it.
		game_state = (game_state | state::building);

		watching = net.id();

		std::array <physics::objects, 4> player_place_objects_info;

		//For host only //Send Objects
		if (!give_players_objects && net.id() == 0)
		{
			players_placed_objects_id.fill({ -1, -1, -1, -1 });
			std::array<int, 4> random_position = { 0, 0, 0, 0 }; // random_indexes();
			for (int i = 0; i < static_cast<int>(player_count); i++)
			{
				glm::vec2 start_position = { 0, 20 + (random_position[i] * 64) };
				placed_objects_list_id = random_picked_object();
				collision_data data;
				int m_id = level.add_object(data, placed_objects_list_id);
				int d_id = physics.add_dynamic_body(start_position, { 0, 0 }, data.width, data.height, { 0, 0 }, placed_objects_list_id);

				//std::cout << "Model ID:\t" << m_id << "\nDynamic ID:\t" << d_id << "Type ID:\t" << placed_objects_list_id << std::endl << std::endl;

				dynamics[d_id].position = start_position;
				dynamics[d_id].velocity = { 0.0f, 0.0f };
				dynamics[d_id].size = { data.width, data.height };
				dynamics[d_id].forces = { 0.0f, 0.0f };
				dynamics[d_id].impulse = { 0.0f, 0.0f };
				dynamics[d_id].dynamic_id = d_id;
				dynamics[d_id].model_id = m_id;
				dynamics[d_id].objects_type_id = placed_objects_list_id;

				dynamics[i].player_moving_object_id = d_id;
				dynamics[i].player_moving_object_type_id = placed_objects_list_id;

				players_placed_objects_id[i] = { dynamics[d_id].dynamic_id, dynamics[d_id].model_id,
							0, dynamics[d_id].objects_type_id };

				total_nr_objects++;
			}

			give_players_objects = true;
		}

		//For clients only //Fetch objects
		if (net.id() != 0 && !give_players_objects)
		{
			players_placed_objects_id.fill({ -1, -1, -1, -1 });
			for (int i = 0; i < static_cast<int>(player_count); i++)
			{
				int obj_id = dynamics[i].player_moving_object_id;
				int obj_type_id = dynamics[i].player_moving_object_type_id;

				if (obj_id != -1 && obj_type_id != -1)
				{
					dynamics[i].player_moving_object_id = -1;
					dynamics[i].player_moving_object_type_id = -1;

					collision_data data;
					int m_id = level.add_object(data, obj_type_id);
					int d_id = physics.add_dynamic_body(dynamics[obj_id].position, { 0, 0 }, data.width, data.height, { 0, 0 }, obj_type_id);

					//std::cout << "Model ID:\t" << m_id << "\nDynamic ID:\t" << d_id << "\nType ID:\t" << obj_type_id << std::endl << std::endl;

					//dynamics[d_id].position = start_position;
					//dynamics[d_id].velocity = { 0.0f, 0.0f };
					dynamics[d_id].size = { data.width, data.height };
					dynamics[d_id].forces = { 0.0f, 0.0f };
					dynamics[d_id].impulse = { 0.0f, 0.0f };
					dynamics[d_id].dynamic_id = d_id;
					dynamics[d_id].model_id = m_id;
					dynamics[d_id].objects_type_id = obj_type_id;

					players_placed_objects_id[i] = { dynamics[d_id].dynamic_id, dynamics[d_id].model_id,
								0, dynamics[d_id].objects_type_id };

					give_players_objects = true;

					total_nr_objects++;
				}
			}
		}
		
		for (int i = 0; i < static_cast<int>(player_count); i++)
		{
			if (players_placed_objects_id[i].place_state != 2)
			{
				//level.moving_models[players_placed_objects_id[i].model_id].set_position(dynamics[players_placed_objects_id[i].dynamics_id].position);
							   
				glm::vec3 pos = physics.get_closest_wall_point(players_placed_objects_id[i].dynamics_id);
						
				//std::cout << pos.z << "\n";
				//float degree = 90.f * pos.z;

				//level.moving_models[players_placed_objects_id[i].model_id].set_rotation(degree);
				level.moving_models[players_placed_objects_id[i].model_id].set_position({ pos.x, pos.y });

				if (!physics.overlapping(players_placed_objects_id[i].dynamics_id) && glm::vec2(pos.x, pos.y) != dynamics[players_placed_objects_id[i].dynamics_id].position)
					players_placed_objects_id[i].place_state = 1;
				else
					players_placed_objects_id[i].place_state = 0;


			}
		}

		//Set State -> pre_playing
		if (!gameplay.build_stage(static_cast<int>(player_count)))
		{
			if (net.id() == 0)
				net_state.state = network::SessionState::pre_playing;
			game_state = (game_state | state::pre_playing);
		}
	}
	else if (net_state.state == network::SessionState::pre_playing)
	{
		if (give_players_objects)
		{
			for (int i = 0; i < static_cast<int>(player_count); i++)
			{
				auto& ppoi = players_placed_objects_id[i];

				if (ppoi.place_state == 0 || ppoi.place_state == 1) 
				{
					//Remove object
					dynamics[ppoi.dynamics_id].position = glm::vec3{ 3000, 0, 0 };
					level.moving_models[ppoi.model_id].set_position(dynamics[ppoi.dynamics_id].position);

					int index = -1;
					for (auto& dyn : dynamics)
					{
						if (dyn.model_id == level.moving_models.size() - 1)
						{
							index = dyn.model_id;
							break;
						}
					}

					std::swap(level.moving_models[ppoi.model_id], level.moving_models[index]);
					level.moving_models.pop_back();
					physics.remove_body(ppoi.dynamics_id);

					dynamics[ppoi.dynamics_id] = dynamics[index];

					/*dynamics[index].model_id = ppoi.model_id;
					dynamics[index].dynamic_id = ppoi.dynamics_id;*/

					total_nr_objects--;
				}
				else
				{
					glm::vec3 pos = physics.get_closest_wall_point(players_placed_objects_id[i].dynamics_id);
					dynamics[players_placed_objects_id[i].dynamics_id].position = { pos.x, pos.y };
					level.moving_models[players_placed_objects_id[i].model_id].set_position({ pos.x, pos.y });
				}
			}

			/*for (int i = 4; i < 8; i++)
			{
				std::cout << "Dynamic: " << dynamics[i].position.x << " - " << dynamics[i].position.y << "\n";
			}*/

			give_players_objects = false;
		}
		
		give_players_objects = false;

		//Begin 3, 2, 1, GO! countdown.
		game_state = (game_state | state::pre_playing);

		if (lua_data.time <= 0.5f)
		{
			watching = net.id();
			if (net.id() == 0)
				net_state.state = network::SessionState::playing;
			game_state = (game_state | state::playing);
		}

		//Set State -> playing
	}
	else if (net_state.state == network::SessionState::playing)
	{
		//RUN!
		game_state = (game_state | state::playing);

		static float all_finished_timer = 6.5f;

		//Set State -> score

		int total_finished = 0;
		for (int i = 0; i < static_cast<int>(player_count); i++)
		{
			if (lua_data.finished[i] || lua_data.died[i])
			{
				total_finished++;
			}
		}

		if (total_finished == static_cast<int>(player_count))
		{
			all_finished_timer -= dt;
			lua_data.time = all_finished_timer;
		}

		if (lua_data.time <= 0.0f)
		{
			all_finished_timer = 3.5f;
			if (net.id() == 0)
				net_state.state = network::SessionState::score;
			game_state = (game_state | state::score);
		}
	}
	else if (net_state.state == network::SessionState::score)
	{
		//Distribute score
		game_state = (game_state | state::score);
		static float score_timer = 3.5f;

		score_timer -= dt;

		//If winner found
			//Set State -> game_over
		if (lua_data.game_over)
		{
			if (net.id() == 0)
				net_state.state = network::SessionState::game_over;
			game_state = (game_state | state::game_over);
		}
		else //Otherwise distribute score and //Set State -> pre_building
		{
			if (score_timer <= 0.0f)
			{
				score_timer = 3.5f; 
				if (net.id() == 0)
					net_state.state = network::SessionState::pre_building;
				game_state = (game_state | state::pre_building);
				//gameplay.new_round();
			}
		}
	}
	else if (net_state.state == network::SessionState::game_over)
	{
		//Announs winner and return to lobby.
		game_state = (game_state | state::game_over);


		if (net.id() == 0)
			net_state.state = network::SessionState::lobby;
		game_state = (game_state | state::lobby);

		load_map("../resources/level/lobby.ssp");
		//Set State -> lobby
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
	
	{
		logic::objects_array obj;
		for (auto i = 0u; i < dynamics.size(); ++i)
		{
			obj[i].position = dynamics[i].position;
			obj[i].velocity = dynamics[i].velocity;
			obj[i].size = dynamics[i].size;
			obj[i].forces = dynamics[i].forces;
			obj[i].impulse = dynamics[i].impulse;
			obj[i].shield_active = dynamics[i].shield_active;
		}

		lua_data = gameplay.update(
			{ delta, obj, triggers,
			player_inputs,
			anim_states,
			players_placed_objects_id,
			static_cast<int>(player_count),
			triggers_types },
			game_state);

		for (auto i = 0u; i < dynamics.size(); ++i)
		{
			dynamics[i].position = obj[i].position;
			dynamics[i].velocity = obj[i].velocity;
			dynamics[i].size = obj[i].size;
			dynamics[i].forces = obj[i].forces;
			dynamics[i].impulse = obj[i].impulse;
			dynamics[i].is_stund = obj[i].is_stund;
			dynamics[i].shield_active = obj[i].shield_active;
		}
	}

	if (net.connected())
	{
		for (int i = 0; i < 4; ++i)
		{
			if (!dynamics[i].is_stund) // test for placing objects script.
			{

			if (level.moving_models[i].is_animated)
			{
				level.moving_models[i].update_animation((float)delta.count(), anim_states[i]);
			}

			if (physics.rw[i] == true)
				level.moving_models[i].rotate({ 0.0f, 1.0f, 0.0f }, glm::radians(180.0f));
			else if (physics.lw[i] == true)
				level.moving_models[i].rotate({ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f));


					if (player_inputs[i][logic::button::right] == logic::button_state::held)
					{
						if (level.moving_models[i].get_state() != anim::hanging_right &&
							level.moving_models[i].get_state() != anim::hanging_left &&
							level.moving_models[i].get_state() != anim::turning &&
							level.moving_models[i].get_state() != anim::connect_wall &&
							level.moving_models[i].get_state() != anim::jump_from_wall)
							level.moving_models[i].rotate({ 0.0f, 1.0f, 0.0f }, glm::radians(180.0f));
					}
					else if (player_inputs[i][logic::button::left] == logic::button_state::held)
					{
						if (level.moving_models[i].get_state() != anim::hanging_right &&
							level.moving_models[i].get_state() != anim::hanging_left &&
							level.moving_models[i].get_state() != anim::turning &&
							level.moving_models[i].get_state() != anim::connect_wall  &&
							level.moving_models[i].get_state() != anim::jump_from_wall)
							level.moving_models[i].rotate({ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f));
					}
				}

				glm::vec2 pos
				{
					dynamics[i].position.x,
					dynamics[i].position.y - dynamics[i].size.y
				};
					
			if (!(game_state & state::building))
			{
				level.v[i] = pos;
				level.moving_models[i].set_position(pos);
			}
		}
	}

	if (game_state & state::building)
	{
		level.v[net.id()] = { level.v[net.id()].x, dynamics[players_placed_objects_id[net.id()].dynamics_id].position.y - 3 };
	}

	anim idle = anim::falling;

	for (auto& model : level.animated_models)
		model.update_animation((float)delta.count(), idle);

	for (int i = 4; i < level.moving_models.size(); i++)
		if(level.moving_models[i].is_animated)
			level.moving_models[i].update_animation((float)delta.count(), idle);

	physics.update(delta, dynamics, triggers, triggers_types, anim_states);

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
		for (int i = 0; i < 4; ++i)
		{
			stream << lua_data.names[i] << ": "
				<< fixed << setprecision(2) 
				<< lua_data.scores[i] << " | ";
		}			
		
		if ((lua_data.died[net.id()] || lua_data.finished[net.id()]) && (net_state.state == network::SessionState::playing))
		{
			//Spectator
			if ((*local_input)[logic::button::right] == logic::button_state::pressed)
			{
				watching = (watching + 1) % (static_cast<int>(player_count));

				if (watching == net.id())
				{
					watching = (watching + 1) % (static_cast<int>(player_count));
				}

				if (lua_data.died[watching] || lua_data.finished[watching])
				{
					watching = (watching + 1) % (static_cast<int>(player_count));
				}
			}

			if ((*local_input)[logic::button::left] == logic::button_state::pressed)
			{
				watching = (watching - 1);
				if (watching < 0)
					watching = static_cast<int>(player_count) - 1;

				if (watching == net.id())
				{
					watching = (watching - 1);
					if (watching < 0)
						watching = static_cast<int>(player_count) - 1;
				}

				if (lua_data.died[watching] || lua_data.finished[watching])
				{
					watching = (watching - 1);
					if (watching < 0)
						watching = static_cast<int>(player_count) - 1;
				}
			}
		}
		
		all_placed_objects.clear();

		for (int i = 4; i < total_nr_objects + 4; i++)
		{
			build_information info;

			info.local_position = glm::vec3(dynamics[i].position.x, dynamics[i].position.y, 0.0f);
			info.object_id = dynamics[i].objects_type_id;

			all_placed_objects.push_back(info);
		}

		std::array<int, 4> moving_objects_id = { players_placed_objects_id[0].model_id,
			players_placed_objects_id[1].model_id,
			players_placed_objects_id[2].model_id,
			players_placed_objects_id[3].model_id };
			
		string temp = stream.str();
		renderer.update(delta,
			obj,
			player_inputs[net.id()].cursor,
			directions,
			chat[1], static_cast<int>(player_count),
			net.id(), game_state, temp, lua_data.died, 
			lua_data.finished, lua_data.scores, lua_data.time, lua_data.goal_height, all_placed_objects,
			watching,
			moving_objects_id);
	}

	if (game_state & state::menu && menu.get_fullscreen_pressed())
	{
		window.toggle_screen_mode(settings);
	}

}

void Game::pack_data()
{	
	for (int i = 0; i < 4; ++i)
	{
		net_state.inputs[i] = player_inputs[i];
	}

	for (auto i = 0u; i < dynamics.size(); ++i)
	{
		net_state.game_objects[i].position = dynamics[i].position;
		net_state.game_objects[i].velocity = dynamics[i].velocity;

		//Vincent
		net_state.game_objects[i].player_moving_object_type_id = dynamics[i].player_moving_object_type_id;
		net_state.game_objects[i].player_moving_object_id = dynamics[i].player_moving_object_id;
	}

	if ((*local_input)[logic::button::refresh] == logic::button_state::held && net.id() == 0)
		net_state.state = network::SessionState::waiting;
}

void Game::unpack_data()
{	
	for (int i = 0; i < 4; ++i)
	{
		if (i != net.id())
		{
			player_inputs[i] = net_state.inputs[i];
		}		
	}
	
	//if (state_sequence != net_state.sequence)
	{
		state_sequence = net_state.sequence;
		player_count = net_state.player_count;

		if (net.id())
		{
			for (auto i = 0u; i < dynamics.size(); ++i)
			{
				dynamics[i].position = net_state.game_objects[i].position;
				dynamics[i].velocity = net_state.game_objects[i].velocity;

				//Vincent
				if (i < 4)
				{
					dynamics[i].player_moving_object_type_id = net_state.game_objects[i].player_moving_object_type_id;
					dynamics[i].player_moving_object_id = net_state.game_objects[i].player_moving_object_id;
				}
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


		level.moving_models[model_id].set_position(dynamics[dynamic_id].position);

	}
}

std::array<int, 4> Game::random_indexes()
{
	std::vector<int> taken = { 0, 1, 2, 3 };
	std::array<int, 4> out;

	int i = 0;
	while (taken.size() > 0)
	{
		int rand_index = rand() % taken.size();
		int index = taken[rand_index];
		taken.erase(taken.begin() + rand_index);
		out[i] = index;
		i++;
	}

	return out;
}

void Game::remove_object(int id)
{
	dynamics[id].position = glm::vec3{ 3000, 0, 0 };
	level.moving_models[id].set_position(dynamics[id].position);
}

void Game::load_map(const char*  file_path)
{
	physics.clear_object();
	level.clear_object();

	for (int i = 0; i < 4; ++i)
		dynamics[i].position = glm::vec2(3.f * i, 2.5f);
	for (int i = 4; i < dynamics.size(); ++i)
		dynamics[i].position = glm::vec2(-20000.f, -20000.f);

	level = graphics::GameScene(file_path, &mesh_lib, &object_lib);

	physics.clear_static_object();
	for (auto& coll : level.coll_data)
	{
		physics.add_static_body(coll.position,
			glm::vec2{ 0.0f,0.0f }, coll.width, coll.height, coll.trigger);
	}

	gameplay.is_new_round = false;
	give_players_objects = false;
	give_players_objects = false;
	watching = net.id();
}