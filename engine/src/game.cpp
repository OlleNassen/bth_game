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
	, renderer{&lobby}
{
	level1.light_level_1();
	level2.light_level_2();
	lobby.light_lobby();

	anim_states[0] = anim::idle;
	anim_states[1] = anim::idle;
	anim_states[2] = anim::idle;
	anim_states[3] = anim::idle;

	window.assign_key(logic::button::up, GLFW_KEY_W);
	window.assign_key(logic::button::left, GLFW_KEY_A);
	window.assign_key(logic::button::down, GLFW_KEY_S);
	window.assign_key(logic::button::right, GLFW_KEY_D);
	window.assign_key(logic::button::jump, GLFW_KEY_SPACE);
	window.assign_key(logic::button::rotate, GLFW_KEY_LEFT_SHIFT);
	window.assign_key(logic::button::refresh, GLFW_KEY_F5);
	window.assign_key(logic::button::score, GLFW_KEY_TAB);
	window.assign_key(logic::button::menu, GLFW_KEY_F1);
	window.assign_key(logic::button::debug, GLFW_KEY_F3);
	window.assign_key(logic::button::quit, GLFW_KEY_ESCAPE);

	window.assign_button(logic::button::up, controller_buttons::up);
	window.assign_button(logic::button::left, controller_buttons::left);
	window.assign_button(logic::button::down, controller_buttons::down);
	window.assign_button(logic::button::right, controller_buttons::right);
	window.assign_button(logic::button::jump, controller_buttons::a);
	window.assign_button(logic::button::rotate, controller_buttons::b);
	window.assign_button(logic::button::select, controller_buttons::y);
	window.assign_button(logic::button::quit, controller_buttons::start);

	window.assign_axis_neg(logic::button::left, controller_axis::ls_right);
	window.assign_axis_pos(logic::button::right, controller_axis::ls_right);

	for (int i = 0; i < 12; ++i)
	{
		player_inputs[0][static_cast<logic::button>(i)] = logic::button_state::none;
		player_inputs[1][static_cast<logic::button>(i)] = logic::button_state::none;
		player_inputs[2][static_cast<logic::button>(i)] = logic::button_state::none;
		player_inputs[3][static_cast<logic::button>(i)] = logic::button_state::none;
	}	

	physics.add_dynamic_body(level->v[0], { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });
	physics.add_dynamic_body(level->v[1], { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });
	physics.add_dynamic_body(level->v[2], { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });
	physics.add_dynamic_body(level->v[3], { 0.0, 1.75 }, 1, 3.5, { 0.0, 0.0 });

	for (int i = 0; i < 4; ++i)
	{
		dynamics[i].position = level->v[i];
		dynamics[i].velocity = { 0.0f, 0.0f };
		dynamics[i].size = { 1.0f, 3.5f };
		dynamics[i].forces = { 0.0f, 0.0f };
		dynamics[i].impulse = { 0.0f, 0.0f };
		dynamics[i].shield_active = false; //trigger
		anim_states[i] = anim::idle;
	}

	for (auto& coll : level->coll_data)
		physics.add_static_body(coll.position, 
			glm::vec2{ 0.0f, 0.0f }, coll.width, coll.height, coll.trigger);


	srand(time(NULL));

	//Start States
	net_state.state = network::SessionState::lobby;

	for (int i = 0; i < 5; i++)
	{
		random_values[i] = 1 + (rand() % 4);
	}
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

		if (d_id != -1)
		{
			info.local_position = glm::vec3(dynamics[d_id].position.x, dynamics[d_id].position.y, 0.0f);
			info.debug_positions = physics.get_debug_for(d_id);
			info.place_state = players_placed_objects_id[i].place_state;
			info.object_id = players_placed_objects_id[i].model_type_id;
			build_info.push_back(info);
		}
	}

	bool view_score = (*local_input)[logic::button::score] == logic::button_state::held;

	std::array<glm::vec2, 4> player_positions;

	for (int i = 0; i < 4; i++)
	{
		player_positions[i] = dynamics[i].position;
	}

	renderer.render(chat.begin(), chat.end(),
		menu.button_strings(),
		db_coll, build_info, lua_data.game_over, lua_data.died, 
		lua_data.finished, lua_data.scores, lua_data.trigger_type, lua_data.time,
		net.id(),
		players_placed_objects_id[net.id()].model_type_id,
		remove_lines,
		view_score,
		player_positions,
		menu.get_how_to_play());
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
		gameplay.refresh();

		load_map(&lobby);
		level_id = 0;
		if (net.id() == 0)
		{
			srand(time(NULL));

			for (int i = 0; i < 5; i++)
			{
				random_values[i] = 1 + (rand() % 4);
			}
		}


		door_1_votes = 0;
		door_2_votes = 0;
	}

	if (menu.on())
		game_state = (game_state | state::menu);

	if (chat.is_on())
		game_state = (game_state | state::chat);

	if (net.connected())
		game_state = (game_state | state::connected);
	
	if ((net_state.state == network::SessionState::lobby))
	{
		if (level != &lobby)
		{
			gameplay.refresh();
			load_map(&lobby);

			level_id = 0;
			//std::cout << level_id << "\n";

			for (int i = 0; i < static_cast<int>(player_count); i++)
			{
				lua_data.finished[i] = false;
			}
			if (net.id() == 0)
			{
				srand(time(NULL));

				for (int i = 0; i < 5; i++)
				{
					random_values[i] = 1 + (rand() % 4);
				}
			}


			door_1_votes = 0;
			door_2_votes = 0;

			turrets.clear();
		}

		game_state = (game_state | state::lobby);
		
		bool all_ready = true;
		for (int i = 0; i < static_cast<int>(player_count); i++)
		{
			if (!lua_data.finished[i])
			{
				all_ready = false;
			}
		}

		if ((*local_input)[logic::button::jump] == logic::button_state::pressed && !(game_state & state::menu) && all_ready)
		{
			if (net.id() == 0)
			{
				for (int i = 0; i < 4; i++)
				{
					if (dynamics[i].position.x < -22)
					{
						door_1_votes++;
					}
					else if (dynamics[i].position.x > 22)
					{
						door_2_votes++;
					}
				}

				if (door_1_votes == door_2_votes)
				{
					if (dynamics[0].position.x < -22)
					{
						door_1_votes++;
					}
					else if (dynamics[0].position.x > 22)
					{
						door_2_votes++;
					}
				}

				if (door_1_votes > door_2_votes)
					level_id = 1;
				else
					level_id = 2;

				net_state.state = network::SessionState::pre_building;
			}
			game_state = (game_state | state::pre_building);
		}
	}

	else if (net_state.state == network::SessionState::pre_building)
	{
		if (level == &lobby)
		{ 
			switch (level_id)
			{
			case 1:
				gameplay.refresh();
				load_map(&level1);

				moving_platform_ids.clear();
				turrets.clear();
				nr_of_moving_platforms = 0;
				add_moving_platforms();

				break;

			case 2:
				gameplay.refresh();
				load_map(&level2);

				moving_platform_ids.clear();
				turrets.clear();
				nr_of_moving_platforms = 0;

				place_random_objects(120, 8);
				break;

			case 0:
				break;
			}
		}
		else
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
	}
	else if (net_state.state == network::SessionState::building)
	{
		for (auto& anim : anim_states)
		{
			anim = anim::falling;
		}

		//Inititate player objects and move it.
		game_state = (game_state | state::building);

		watching = net.id();

		std::array <physics::objects, 4> player_place_objects_info;

		//For host only //Send Objects
		if (!give_players_objects && net.id() == 0)
		{
			players_placed_objects_id.fill({ -1, -1, -1, -1 });
			std::array<int, 4> indexies = random_indexes();

			/*std::cout << "New Round:" <<
				"\nRandom Indexies: " << indexies[0] << ", " << indexies[1] << ", " << indexies[2] << ", " << indexies[3] << "\n\n";*/

			std::array<glm::vec2, 4> start_positons = { glm::vec2{0.f, 60.5f}, glm::vec2{0.f, 113.5}, glm::vec2{0.f, 167.f}, glm::vec2{0.f, 224.5f} };
			for (int i = 0; i < static_cast<int>(player_count); i++)
			{
				glm::vec2 start_position = start_positons[indexies[i]];
				//This is set to always spawn a turrent
				int type_id = random_picked_object();

				collision_data data;
				int m_id = level->add_object(data, type_id);
				int d_id = physics.add_dynamic_body(start_position, { 0, 0 }, data.width, data.height, { 0, 0 }, type_id);

				/*if (net.id() == i)
					std::cout << "This is me:\n";

				std::cout << "Player:\t" << i <<
					"\nModel ID:\t" << m_id << 
					"\nDynamic ID:\t" << d_id << 
					"\nType ID:\t" << type_id <<
					"\n\n";*/

				dynamics[d_id].position = start_position;
				dynamics[d_id].velocity = { 0.0f, 0.0f };
				dynamics[d_id].size = { data.width, data.height };
				dynamics[d_id].forces = { 0.0f, 0.0f };
				dynamics[d_id].impulse = { 0.0f, 0.0f };
				dynamics[d_id].dynamic_id = d_id;
				dynamics[d_id].model_id = m_id;
				dynamics[d_id].objects_type_id = type_id;

				dynamics[i].player_moving_object_id = d_id;
				dynamics[i].player_moving_object_type_id = type_id;

				players_placed_objects_id[i] = { dynamics[d_id].dynamic_id, dynamics[d_id].model_id,
							0, dynamics[d_id].objects_type_id };
				
				total_nr_objects++;
			}

			turret_infos.fill({ 0, 0 });
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
					//dynamics[i].player_moving_object_id = -1;
					//dynamics[i].player_moving_object_type_id = -1;

					glm::vec2 start_position = dynamics[obj_id].position;

					collision_data data;
					int m_id = level->add_object(data, obj_type_id);
					int d_id = physics.add_dynamic_body(start_position, { 0, 0 }, data.width, data.height, { 0, 0 }, obj_type_id);

					if (obj_id != d_id)
					{
						std::cout << "WARNING! - Invalid dynamic id! \nERROR: game.cpp - Row: 386\n";
					}

					/*if (net.id() == i)
						std::cout << "This is me:\n";

					std::cout << "Player:\t" << i <<
						"\nModel ID:\t" << m_id <<
						"\nDynamic ID:\t" << d_id <<
						"\nType ID:\t" << obj_type_id <<
						"\n\n";*/

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

			turret_infos.fill({ 0, 0 });
		}
		
		for (int i = 0; i < static_cast<int>(player_count); i++)
		{
			if (players_placed_objects_id[i].place_state != 2 && 
				players_placed_objects_id[i].dynamics_id != -1 && 
				give_players_objects)
			{							   
				glm::vec3 pos = physics.get_closest_wall_point(players_placed_objects_id[i].dynamics_id, dynamics[players_placed_objects_id[i].dynamics_id].position);

				float degree = 90.f * pos.z;

				physics.set_rotation(players_placed_objects_id[i].dynamics_id, static_cast<int>(pos.z));

				physics.set_body_position(players_placed_objects_id[i].dynamics_id, { pos.x, pos.y });

				if (players_placed_objects_id[i].model_type_id == 1)
				{
					if (pos.z == 1)
					{
						if ((player_inputs[i])[logic::button::up] == logic::button_state::held)
						{
							turret_infos[i].direction = 1;
						}
						else if ((player_inputs[i])[logic::button::down] == logic::button_state::held)
						{
							turret_infos[i].direction = 0;
						}

						if (turret_infos[i].direction == 0)
							level->moving_models[players_placed_objects_id[i].model_id].set_rotation({ 1, 0, 1 }, { 0, 0, degree });
						else if (turret_infos[i].direction == 1)
							level->moving_models[players_placed_objects_id[i].model_id].set_rotation({ 1, 0, 1 }, { 180.f, 0, degree });
					}
					else if (pos.z == 2)
					{
						if ((player_inputs[i])[logic::button::right] == logic::button_state::held)
						{
							turret_infos[i].direction = 1;
						}
						else if ((player_inputs[i])[logic::button::left] == logic::button_state::held)
						{
							turret_infos[i].direction = 0;
						}

						if (turret_infos[i].direction == 0)
							level->moving_models[players_placed_objects_id[i].model_id].set_rotation({ 0, 1, 1 }, { 0, 180.f, degree });
						else if (turret_infos[i].direction == 1)
							level->moving_models[players_placed_objects_id[i].model_id].set_rotation({ 0, 1, 1 }, { 0, 0, degree });
					}
					else if (pos.z == 3)
					{
						if ((player_inputs[i])[logic::button::up] == logic::button_state::held)
						{
							turret_infos[i].direction = 1;
						}
						else if ((player_inputs[i])[logic::button::down] == logic::button_state::held)
						{
							turret_infos[i].direction = 0;
						}

						if (turret_infos[i].direction == 0)
							level->moving_models[players_placed_objects_id[i].model_id].set_rotation({ 1, 0, 1 }, { 180.f, 0, degree });
						else if (turret_infos[i].direction == 1)
							level->moving_models[players_placed_objects_id[i].model_id].set_rotation({ 1, 0, 1 }, { 0, 0, degree });
					}
					else if (pos.z == 4)
					{
						if ((player_inputs[i])[logic::button::right] == logic::button_state::held)
						{
							turret_infos[i].direction = 1;
						}
						else if ((player_inputs[i])[logic::button::left] == logic::button_state::held)
						{
							turret_infos[i].direction = 0;
						}

						if (turret_infos[i].direction == 0)
							level->moving_models[players_placed_objects_id[i].model_id].set_rotation({ 0, 1, 1 }, { 0, 0, degree });
						else if (turret_infos[i].direction == 1)
							level->moving_models[players_placed_objects_id[i].model_id].set_rotation({ 0, 1, 1 }, { 0, 180.f, degree });
					}
				}
				else
					level->moving_models[players_placed_objects_id[i].model_id].set_rotation({ 0, 0, 1 }, { 0, 0, degree });

				level->moving_models[players_placed_objects_id[i].model_id].set_position({ pos.x, pos.y });

				if (!physics.overlapping(players_placed_objects_id[i].dynamics_id) && glm::vec2(pos.x, pos.y) != dynamics[players_placed_objects_id[i].dynamics_id].position)
				{
					players_placed_objects_id[i].place_state = 1;
					if (net.id() == 0)
						has_placed_correctly[i] = 0;
				}
				else
				{
					players_placed_objects_id[i].place_state = 0; 
					if (net.id() == 0)
						has_placed_correctly[i] = 0;
				}
			}
			
			if (players_placed_objects_id[i].place_state == 2)
			{
				if (net.id() == 0)
					has_placed_correctly[i] = 1;
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
		if (give_players_objects && lua_data.time < 2.f)
		{
			for (int i = 0; i < static_cast<int>(player_count); i++)
			{
				auto& ppoi = players_placed_objects_id[i];
			
				//if (ppoi.place_state == 0 || ppoi.place_state == 1) 

				if (has_placed_correctly[i] == 0)
				{
					/*std::cout << "Removed: " << i <<
						"\nModel ID:\t" << ppoi.model_id <<
						"\nDynamic ID:\t" << ppoi.dynamics_id <<
						"\nType ID:\t" << ppoi.model_type_id <<
						"\n\n";*/

					//Remove object
					dynamics[ppoi.dynamics_id].position = glm::vec3{ 3000, 0, 0 };
					level->moving_models[ppoi.model_id].set_position(dynamics[ppoi.dynamics_id].position);

					int index = -1;
					for (auto& dyn : dynamics)
					{
						if (dyn.model_id == level->moving_models.size() - 1)
						{
							index = dyn.model_id;
							break;
						}
					}

					level->moving_models.erase(level->moving_models.begin() + ppoi.model_id);

					physics.remove_body(ppoi.dynamics_id);

					auto beg = dynamics.begin() + players_placed_objects_id[i].dynamics_id;
					std::rotate(beg, beg + 1, dynamics.end());

					for (int i = 0; i < static_cast<int>(player_count); i++)
					{
						players_placed_objects_id[i].model_id--;
						players_placed_objects_id[i].dynamics_id--;
					}

					total_nr_objects--;
				}
				else
				{
					glm::vec3 pos = physics.get_closest_wall_point(players_placed_objects_id[i].dynamics_id, dynamics[players_placed_objects_id[i].dynamics_id].position);
					dynamics[players_placed_objects_id[i].dynamics_id].position = { pos.x, pos.y };
					level->moving_models[players_placed_objects_id[i].model_id].set_position({ pos.x, pos.y });

					if (players_placed_objects_id[i].model_type_id == 1)
					{
						turret_infos[i].direction;
						turret_infos[i].rotation = static_cast<int>(pos.z);

						add_turret(players_placed_objects_id[i].dynamics_id, turret_infos[i], dynamics[players_placed_objects_id[i].dynamics_id].position);

						//std::cout << turret_infos[i].rotation << " : " << turret_infos[i].direction << "\n";
						

					}
				}
			}

			/*for (int i = 4; i < 8; i++)
			{
				std::cout << "Dynamic: " << dynamics[i].position.x << " - " << dynamics[i].position.y << "\n";
			}*/

			give_players_objects = false;
		}
		
		//give_players_objects = false;

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
			if (lua_data.finished[i] && !lua_data.died[i])
			{
				total_finished++;
			}
		}

		if (total_finished == static_cast<int>(player_count))
		{
			all_finished_timer -= dt;
			lua_data.time = all_finished_timer;
		}
		else
			all_finished_timer = 6.5f;

		if (lua_data.time <= 0.0f)
		{
			all_finished_timer = 6.5f;
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

		//std::cout << "game over man" << "\n";
		if ((*local_input)[logic::button::jump] == logic::button_state::pressed) 
		{
			if (net.id() == 0)
				net_state.state = network::SessionState::lobby;
			game_state = (game_state | state::lobby);
			load_map(&lobby);
			//Set State -> lobby
		}
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
		is_client = net.id();
	}

	player_hit_array = { false, false, false, false };

	if (turretframe >= 30 && turretframe <= 32)
	{
		
		//Update laser draw here....bool draw laser
		for (auto i = 0; i < turrets.size(); ++i)
		{
			laser_update(turrets[i], player_hit_array);
		}
	}

	
	buff_activ = { -1, -1, -1, -1 };

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
		
		if (!menu.on())
		{
			lua_data = gameplay.update(
				{ delta, obj, triggers,
				player_inputs,
				anim_states,
				players_placed_objects_id,
				static_cast<int>(player_count),
				spikeframe,
				turretframe,
				triggers_types,
				player_hit_array,
				random_values,
				buff_activ,
				moving_platform_ids },
				game_state, physics.rw, physics.lw, net.id());
		}

		//add laser hit array

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

			if (level->moving_models[i].is_animated)
			{
				level->moving_models[i].update_animation((float)delta.count(), anim_states[i]);
			}
			if (!dynamics[i].is_stund) // test for placing objects script.
			{

			if (physics.rw[i] == true)
				level->moving_models[i].rotate({ 0.0f, 1.0f, 0.0f }, glm::radians(180.0f));
			else if (physics.lw[i] == true)
				level->moving_models[i].rotate({ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f));


					if (player_inputs[i][logic::button::right] == logic::button_state::held)
					{
						if (level->moving_models[i].get_state() != anim::hanging_right &&
							level->moving_models[i].get_state() != anim::hanging_left &&
							level->moving_models[i].get_state() != anim::turning &&
							level->moving_models[i].get_state() != anim::connect_wall &&
							level->moving_models[i].get_state() != anim::jump_from_wall)
							level->moving_models[i].rotate({ 0.0f, 1.0f, 0.0f }, glm::radians(180.0f));
					}
					else if (player_inputs[i][logic::button::left] == logic::button_state::held)
					{
						if (level->moving_models[i].get_state() != anim::hanging_right &&
							level->moving_models[i].get_state() != anim::hanging_left &&
							level->moving_models[i].get_state() != anim::turning &&
							level->moving_models[i].get_state() != anim::connect_wall  &&
							level->moving_models[i].get_state() != anim::jump_from_wall)
							level->moving_models[i].rotate({ 0.0f, 1.0f, 0.0f }, glm::radians(0.0f));
					}
				}

				glm::vec2 pos
				{
					dynamics[i].position.x,
					dynamics[i].position.y - dynamics[i].size.y
				};
					
			if (!(game_state & state::building))
			{
				level->v[i] = pos;
				level->moving_models[i].set_position(pos);
			}
		}
		anim idle = anim::falling;

		bool spike = false, jump = false, speed = false, random = false, shield = false, turret = false, stun = false;

		for (int i = 4; i < level->moving_models.size(); i++)
			if (level->moving_models[i].is_animated)
			{
				if (level->moving_models[i].mesh->name == "spike_trap" && spike == false ||
					level->moving_models[i].mesh->name == "double_jump" && jump == false ||
					level->moving_models[i].mesh->name == "turret" && turret == false ||
					level->moving_models[i].mesh->name == "stun_trap" && stun == false ||
					level->moving_models[i].mesh->name == "speed_boost" && speed == false ||
					level->moving_models[i].mesh->name == "random_buff" && random == false ||
					level->moving_models[i].mesh->name == "shield" && shield == false)
				{
					level->moving_models[i].update_animation((float)delta.count(), idle);
					if (level->moving_models[i].mesh->name == "spike_trap")
					{
						spike = true;
						spikeframe = level->moving_models[i].getCurrentKeyframe();
					}
					if (level->moving_models[i].mesh->name == "double_jump")
						jump = true;
					if (level->moving_models[i].mesh->name == "turret")
					{
						turret = true;
						turretframe = level->moving_models[i].getCurrentKeyframe();
					}
					if (level->moving_models[i].mesh->name == "stun_trap")
						stun = true;
					if (level->moving_models[i].mesh->name == "random_buff")
						random = true;
					if (level->moving_models[i].mesh->name == "shield")
						shield = true;
					if (level->moving_models[i].mesh->name == "speed_boost")
						speed = true;
				}	  	
			}		   
	}				   

	if ((game_state & state::building) && players_placed_objects_id[net.id()].dynamics_id != -1)
	{
		level->v[net.id()] = { level->v[net.id()].x, dynamics[players_placed_objects_id[net.id()].dynamics_id].position.y - 3 };
	}

	if (!(game_state & state::building))
		physics.update(delta, dynamics, triggers, triggers_types, anim_states);

	if (!(game_state & state::lobby))
		str = 0;
	
	pack_data();
	net.update(net_state, str);
	unpack_data();

	if (!is_client)
		chat[1] = "";

	{
		graphics::objects_array obj;
		for (auto i = 0u; i < dynamics.size(); ++i)
		{
			obj[i].position = dynamics[i].position;
			obj[i].size = dynamics[i].size;
			obj[i].bullet_hit = !dynamics[i].shield_active;
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
		
		if ((!lua_data.died[net.id()] && lua_data.finished[net.id()]) && (net_state.state == network::SessionState::playing))
		{
			//Spectator
			if ((*local_input)[logic::button::right] == logic::button_state::pressed)
				watching = find_next_spectator(watching);

			if ((*local_input)[logic::button::left] == logic::button_state::pressed)
				watching = find_previous_spectator(watching);
		}
		
		all_placed_objects.clear();

		for (int i = 4 + nr_of_moving_platforms; i < total_nr_objects + 4 + nr_of_moving_platforms; i++)
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
			
		bool view_score = (*local_input)[logic::button::score] == logic::button_state::held;

		std::vector<glm::vec2> laser_start;
		std::vector<glm::vec2> laser_end;
		
		for (int i = 0; i < turrets.size(); ++i)
		{
			laser_start.push_back(turrets[i].barrel_position);
			laser_end.push_back(turrets[i].end_position);
		}

		renderer.update(delta,
			obj,
			player_inputs[net.id()].cursor,
			directions,
			chat[1], static_cast<int>(player_count),
			net.id(), game_state, lua_data.died,
			lua_data.finished, lua_data.scores, lua_data.trigger_type, buff_activ, lua_data.time, lua_data.goal_height, all_placed_objects,
			watching,
			moving_objects_id,
			view_score, lua_data.dash_timer, 
			turretframe, laser_start, laser_end);	
	}

	if (nr_of_moving_platforms != 0)
	{
		graphics::objects_array platform_obj;
		for (auto i = 0; i < moving_platform_ids.size(); ++i)
		{

			platform_obj[i].position = dynamics[moving_platform_ids[i]].position;
			platform_obj[i].size = dynamics[moving_platform_ids[i]].size;
		}

		renderer.update_moving_platforms(platform_obj, dynamics[moving_platform_ids[0]].model_id, moving_platform_ids.size());
	}


	if (game_state & state::menu && menu.get_fullscreen_pressed())
	{
		window.toggle_screen_mode(settings);
	}

}

void Game::pack_data()
{	
	for (int i = 0; i < 4; ++i) //Players
	{
		net_state.inputs[i] = player_inputs[i];
		if (net.id() == 0)
			net_state.has_placed_correctly[i] = has_placed_correctly[i];
	}

	for (auto i = 0u; i < dynamics.size(); ++i)
	{
		net_state.game_objects[i].position = dynamics[i].position;
		net_state.game_objects[i].velocity = dynamics[i].velocity;

		if (net.id() == 0)
		{
			if (i < 5)
				net_state.random_values[i] = random_values[i];
			net_state.level_id = level_id;
		}

		for (int i = 0; i < 4; ++i) //Players
		{
			net_state.game_objects[i].player_moving_object_type_id = dynamics[i].player_moving_object_type_id;
			net_state.game_objects[i].player_moving_object_id = dynamics[i].player_moving_object_id;
		}
	}

	for (auto i = 0u; i < dynamics.size(); ++i)	//Player + Objects
	{
		net_state.game_objects[i].position = dynamics[i].position;
		net_state.game_objects[i].velocity = dynamics[i].velocity;
	}

	if ((*local_input)[logic::button::refresh] == logic::button_state::held && net.id() == 0 && net_state.state != network::SessionState::lobby)
		net_state.state = network::SessionState::waiting;
}

void Game::unpack_data()
{	
	
	for (int i = 0; i < 4; ++i)
	{
		if (i != net.id())
		{
			player_inputs[i] = net_state.inputs[i];
			level_id = net_state.level_id;
		}

		has_placed_correctly[i] = net_state.has_placed_correctly[i];

		dynamics[i].player_moving_object_type_id = net_state.game_objects[i].player_moving_object_type_id;
		dynamics[i].player_moving_object_id = net_state.game_objects[i].player_moving_object_id;
	}


	
	//if (state_sequence != net_state.sequence)
	{
		state_sequence = net_state.sequence;
		player_count = net_state.player_count;

		if (net.id() != 0)
		{
			for (auto i = 0u; i < dynamics.size(); ++i) //Players + objects
			{
				dynamics[i].position = net_state.game_objects[i].position;
				dynamics[i].velocity = net_state.game_objects[i].velocity;

				if (i < 5)
					random_values[i] = net_state.random_values[i];

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

void Game::place_random_objects(float start_height, int number_of_randoms)
{
	srand(random_values[0] + random_values[1] + random_values[2] + random_values[3]);

	collision_data data;

	glm::vec2 startPosition = { 0.0, start_height };
	std::vector<glm::vec2> positions;
	float width_of_map = 40;

	int total_x = 5;
	int total_y = 4;

	int width = static_cast<int>(width_of_map / total_x);

	startPosition.x = (width_of_map * 0.5) - (width / 2);

	for (int i = 0; i < total_y; i++)
	{
		for (int j = 0; j < total_x; j++)
		{
			if ((i % 2) == 1)
			{
				positions.push_back({ (startPosition.x - (j * width) - 1),  startPosition.y + (i * 6) });
			}
			else
			{
				positions.push_back({ (startPosition.x - (j * width) + 1),  startPosition.y + (i * 6) });
			}
			
		}
	}

	std::vector<int> rand_numb;

	bool same_number = false;
	int radom_number;

	if (number_of_randoms > 10)
	{
		number_of_randoms = 10;
	}

	for (int i = 0; i < total_y; i++)
	{
		radom_number = rand() % 5 + (i * 5);
		rand_numb.push_back(radom_number);

		//std::cout << radom_number << std::endl;
	}

	while (rand_numb.size() < number_of_randoms)
	{
		same_number = false;
		radom_number = rand() % positions.size();

		for (int i = 0; i < rand_numb.size(); i++)
		{
			if (radom_number == rand_numb[i])
			{
				same_number = true;
			}
		}
		if (!same_number)
		{
			rand_numb.push_back(radom_number);
		}
	}

	int platform_id = 8;

	for (int i = 0; i < number_of_randoms; i++)
	{
		collision_data data;

		int model_id = level->add_object(data, platform_id);
		data.position = positions[rand_numb[i]];
		level->moving_models[model_id].set_position(data.position);

		physics.add_static_body(data.position,
			glm::vec2{ 0.0f, 0.0f }, data.width, data.height, false);

	}

	auto beg = level->models.begin();
	beg += 9;

	std::sort(beg, level->models.end(), [](const auto& left, const auto& right)
	{
		return left.get_y_position() < right.get_y_position();
	});
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
	level->moving_models[id].set_position(dynamics[id].position);
}

void Game::load_map(graphics::GameScene* scene)
{
	physics.clear_object();
	level->clear_object();

	for (int i = 0; i < 4; ++i)
		dynamics[i].position = glm::vec2(3.f * i, 2.5f);
	for (int i = 4; i < dynamics.size(); ++i)
		dynamics[i].position = glm::vec2(-20000.f, -20000.f);
	
	/*if (net.id() == 0)
	{
		if (scene == &lobby)
			level_id = 0;
		else if (scene == &level1)
			level_id = 1;
		else if (scene == &level2)
			level_id = 2;
	}*/
	
	renderer.switch_scene(scene);
	level = scene;

	physics.clear_static_object();
	for (auto& coll : scene->coll_data)
	{
		physics.add_static_body(coll.position,
			glm::vec2{ 0.0f,0.0f }, coll.width, coll.height, coll.trigger);
	}

	gameplay.is_new_round = false;
	give_players_objects = false;
	give_players_objects = false;
	watching = net.id();
}

void Game::add_moving_platforms()
{
	{
		glm::vec2 start_position = { -12, 150 };
		int objects_list_id = 8;
		collision_data data;
		int m_id = level->add_object(data, objects_list_id);
		int d_id = physics.add_dynamic_body(start_position, { 0, 0 }, data.width, data.height, { 0, 0 }, objects_list_id);

		dynamics[d_id].position = start_position;
		dynamics[d_id].velocity = { 0.0f, 0.0f };
		dynamics[d_id].size = { data.width, data.height };
		dynamics[d_id].forces = { 0.0f, 0.0f };
		dynamics[d_id].impulse = { 0.0f, 0.0f };
		dynamics[d_id].dynamic_id = d_id;
		dynamics[d_id].model_id = m_id;
		dynamics[d_id].objects_type_id = objects_list_id;

		level->moving_models[m_id].set_position(start_position);

		moving_platform_ids.push_back(d_id);

		nr_of_moving_platforms++;
	}

	{
		glm::vec2 start_position = { 0, 150 - 9};
		int objects_list_id = 8;
		collision_data data;
		int m_id = level->add_object(data, objects_list_id);
		int d_id = physics.add_dynamic_body(start_position, { 0, 0 }, data.width, data.height, { 0, 0 }, objects_list_id);

		dynamics[d_id].position = start_position;
		dynamics[d_id].velocity = { 0.0f, 0.0f };
		dynamics[d_id].size = { data.width, data.height };
		dynamics[d_id].forces = { 0.0f, 0.0f };
		dynamics[d_id].impulse = { 0.0f, 0.0f };
		dynamics[d_id].dynamic_id = d_id;
		dynamics[d_id].model_id = m_id;
		dynamics[d_id].objects_type_id = objects_list_id;

		level->moving_models[m_id].set_position(start_position);

		moving_platform_ids.push_back(d_id);

		nr_of_moving_platforms++;
	}

	{
		glm::vec2 start_position = { 12, 150 - 12};
		int objects_list_id = 8;
		collision_data data;
		int m_id = level->add_object(data, objects_list_id);
		int d_id = physics.add_dynamic_body(start_position, { 0, 0 }, data.width, data.height, { 0, 0 }, objects_list_id);

		dynamics[d_id].position = start_position;
		dynamics[d_id].velocity = { 0.0f, 0.0f };
		dynamics[d_id].size = { data.width, data.height };
		dynamics[d_id].forces = { 0.0f, 0.0f };
		dynamics[d_id].impulse = { 0.0f, 0.0f };
		dynamics[d_id].dynamic_id = d_id;
		dynamics[d_id].model_id = m_id;
		dynamics[d_id].objects_type_id = objects_list_id;

		level->moving_models[m_id].set_position(start_position);

		moving_platform_ids.push_back(d_id);

		nr_of_moving_platforms++;
	}
}

int Game::find_next_spectator(int current_id)
{
	for (int i = 0; i < static_cast<int>(player_count); i++)
	{
		current_id = (current_id + 1) % (static_cast<int>(player_count));

		if (current_id != net.id() && (!lua_data.finished[current_id]))
		{
			break;
		}
	}
	return current_id;
}

int Game::find_previous_spectator(int current_id)
{
	for (int i = 0; i < static_cast<int>(player_count); i++)
	{
		current_id = (current_id - 1);
		if (current_id < 0)
			current_id = static_cast<int>(player_count) - 1;

		if (current_id != net.id() && (!lua_data.finished[current_id]))
		{
			break;
		}
	}
	return current_id;
}


void Game::laser_update(turret turret, std::array<bool, 4>& hit_array)
{
	physics.laser_ray_cast(glm::vec3{ turret.barrel_position, 0 }, glm::vec3{ turret.direction, 0 }, turret.range, hit_array);
}

void Game::add_turret(const int dyn_id, const turret_info dir_info, const glm::vec2 turret_pos)
{	
	glm::vec2 direction;
	glm::vec2 barrel_position = turret_pos;
	glm::vec2 end_pos;
	turret temp;
	float temp_range;

	if (dir_info.rotation == 1)//attach to R wall
	{
		if (dir_info.direction == 1) //look UP
		{
			direction = glm::vec2{ 0, 1 };

			barrel_position += glm::vec2{ -0.833, dynamics[dyn_id].size.y / 2 };
		}
		else if (dir_info.direction == 0) //look DOWN
		{
			direction = glm::vec2{ 0, -1 };

			barrel_position -= glm::vec2{ 0.833, dynamics[dyn_id].size.y / 2 };
		}
	}
	else if (dir_info.rotation == 2) //attach to roof
	{
		if (dir_info.direction == 1) //look RIGHT
		{
			direction = glm::vec2{ 1, 0 };

			barrel_position += glm::vec2{ dynamics[dyn_id].size.x / 2, -0.833 };
		}
		else if (dir_info.direction == 0) //look LEFT
		{
			direction = glm::vec2{ -1, 0 };

			barrel_position -= glm::vec2{ dynamics[dyn_id].size.x / 2, 0.833 };
		}
	}
	else if (dir_info.rotation == 3) //attach to L wall
	{
		if (dir_info.direction == 1) //look UP
		{
			direction = glm::vec2{ 0, 1 };

			barrel_position += glm::vec2{ 0.833, dynamics[dyn_id].size.y / 2 };
		}
		else if (dir_info.direction == 0) //look DOWN
		{
			direction = glm::vec2{ 0, -1 };

			barrel_position -= glm::vec2{ -0.833, dynamics[dyn_id].size.y / 2 };
		}
	}
	else if (dir_info.rotation == 4) //attach to floor
	{
		if (dir_info.direction == 1) //look RIGHT
		{
			direction = glm::vec2{ 1, 0 };

			barrel_position += glm::vec2{ dynamics[dyn_id].size.x / 2, 0.833 };
		}
		else if (dir_info.direction == 0) //look LEFT
		{
			direction = glm::vec2{ -1, 0 };

			barrel_position -= glm::vec2{ dynamics[dyn_id].size.x / 2, -0.833 };
		}
	}

	temp_range = physics.laser_range(glm::vec3{ barrel_position, 0 }, glm::vec3{ direction, 0 });
	end_pos = barrel_position + (direction * temp_range);

	temp = turret{ dyn_id, direction, barrel_position, end_pos, temp_range };

	turrets.push_back(temp);

}