#include "renderer.hpp"

#include <iostream>
#include <algorithm>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include <iomanip>

#include <d3d11_4.h>
#include <dxgi1_6.h>

#pragma comment(lib, "dxgi.lib")

#include <psapi.h>
#include <string>

void vramUsage()
{
	IDXGIFactory* dxgifactory = nullptr;
	HRESULT ret_code = ::CreateDXGIFactory(
		__uuidof(IDXGIFactory),
		reinterpret_cast<void**>(&dxgifactory));

	if (SUCCEEDED(ret_code))
	{
		IDXGIAdapter* dxgiAdapter = nullptr;

		if (SUCCEEDED(dxgifactory->EnumAdapters(0, &dxgiAdapter)))
		{
			IDXGIAdapter4* dxgiAdapter4 = NULL;
			if (SUCCEEDED(dxgiAdapter->QueryInterface(__uuidof(IDXGIAdapter4), (void**)&dxgiAdapter4)))
			{
				DXGI_QUERY_VIDEO_MEMORY_INFO info;

				if (SUCCEEDED(dxgiAdapter4->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &info)))
				{
					float memoryUsage = float(info.CurrentUsage / 1024.0 / 1024.0); //MiB

					char msg[100];
					sprintf_s(msg, "%.2f MiB used", memoryUsage);
					MessageBoxA(0, msg, "VRAM", 0);
				};

				dxgiAdapter4->Release();
			}
			dxgiAdapter->Release();
		}
		dxgifactory->Release();
	}
}

void ramUsage()
{
	//src: https://docs.microsoft.com/en-us/windows/desktop/api/psapi/ns-psapi-_process_memory_counters

	DWORD currentProcessID = GetCurrentProcessId();

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, currentProcessID);

	if (NULL == hProcess)
		return;

	PROCESS_MEMORY_COUNTERS pmc{};
	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
	{
		//PagefileUsage is the:
			//The Commit Charge value in bytes for this process.
			//Commit Charge is the total amount of memory that the memory manager has committed for a running process.

		float memoryUsage = float(pmc.PagefileUsage / 1024.0 / 1024.0); //MiB

		char msg[100];
		sprintf_s(msg, "%.2f MiB committed", memoryUsage);
		MessageBoxA(0, msg, "RAM", 0);
	}

	CloseHandle(hProcess);
}

namespace graphics
{

using namespace std::chrono_literals;

ModelsToRender::ModelsToRender(float player_y, Model* begin, Model* end)
{
	first = begin;
	last = end - 1;

	float culling_distance = 50.0f;
	float bottom = player_y - culling_distance;
	float top = player_y + culling_distance;

	for (auto it = begin; it != end; ++it)
	{
		first = it;
		if (it->get_y_position() > bottom)
			break;
	}

	for (auto it = first; it != end; ++it)
	{
		last = it;	
		if (it->get_y_position() > top)
			break;
	}
}

Renderer::Renderer(GameScene* scene)
	: db_camera{glm::radians(90.0f), 1920.f / 1080.f, 0.1f, 100.f}
	, game_camera{glm::radians(65.0f), 1920.f / 1080.f, 0.1f, 100.f}
	, scene{scene}
	, leaderboard(projection)
{
	grid.calculate_grid(game_camera);
	db_camera.position.z = 20.0f;
	glViewport(0, 0, 1920, 1080); // don't forget to configure the viewport to the capture dimensions.

	dir_light.direction = glm::vec3(0, -0.7, -1);
	dir_light.color = glm::vec3(1.0, 0.8, 0.8);
	dir_light.intensity = 0.7f;
}

void Renderer::render(
	const std::string* begin,
	const std::string* end,
	const std::array<std::string, 12>& buttons,
	const std::vector<glm::vec3>& debug_positions,
	const std::vector<build_information>& build_info,
	bool game_over,
	std::array<bool, 4> died,
	std::array<bool, 4> finish,
	std::array<float, 4> scores,
	std::array<int, 4> trigger_type,
	float print_time,
	int player_id,
	int player_object_id,
	std::vector<glm::vec3> remove_lines,
	bool view_score,
	std::array<glm::vec2, 4> player_positions,
	bool how_to_play)const
{
	bool is_menu = (game_state & state::menu);
	bool connected = (game_state & state::connected);
	bool debug_active = (game_state & state::render_physics);
	const float screen_width = 1920.f;
	const float screen_height = 1080.f;
	
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene_texture.bind_framebuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (print_time < 0.0)
		print_time = 0.0f;

	if (!is_menu && connected || is_menu)
	{
		if (!is_menu)
		{
			render_character(robot_shader, game_camera, scene->moving_models, player_count); 
		}

		if (scene->moving_models.size() > 4)
			render_type(pbra, game_camera, &scene->moving_models[4], &scene->moving_models.back() + 1);

		render_type(pbra, game_camera,a_to_render.begin(), a_to_render.end());
		render_type(pbr, game_camera, s_to_render.begin(), s_to_render.end());
		render_type(pbr, game_camera,&scene->models[0], &scene->models[9]);
		laser.render(turret_laser, game_camera);

		
		if (!(game_state & state::lobby))
		{
			fx_emitter.render_particles(fx_dust, fx_bubble, fx_spark, fx_steam, fx_blitz, fx_fire, fx_godray, fx_gust, fx_stun, game_camera, fx_emitter.timer, current_map);
		}
		if (debug_active)
		{
			glDisable(GL_DEPTH_TEST);
			lines.use();
			lines.uniform("projection", game_camera.projection);
			lines.uniform("view", game_camera.view());
			lines.uniform("line_color", glm::vec3(0.2f, 1.0f, 0.2f));
			point_debug(debug_positions);
			glEnable(GL_DEPTH_TEST);
		}
		
	}

	// Post Processing Effects
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Text rendering
	{ 
		post_proccessing.use();
		post_proccessing.uniform("scene_texture", 0);
		post_proccessing.uniform("depth_texture", 1);
		post_proccessing.uniform("screen_warning", 2);

		scene_texture.bind_texture(0);
		scene_texture.bind_texture(1);
		post_processing_effects.texture.bind(2);

		post_proccessing.uniform("pulse", post_processing_effects.glow_value);
		post_processing_effects.render();

		overlays.render(overlay_shader, how_to_play);

		if (game_state & state::pre_building)
		{
			glDisable(GL_DEPTH_TEST);
			if (build_stage_screen.transparency > 0.0f) // (!build_stage_screen.transparency < 0.0005f)
			{
				build_stage_screen.render(build_stage_screen_shader);
			}
			glEnable(GL_DEPTH_TEST);
		}


		glDisable(GL_DEPTH_TEST);
		std::stringstream out_text;
		out_text << std::fixed << std::setprecision(1) << print_time;

		if (game_state & state::lobby && !is_menu)
		{

			text_shader.use();
			text_shader.uniform("projection", projection);
			text_shader.uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));
			
			int total_players_ready = 0;
			int level_1_index = 0;
			int level_2_index = 0;
			for (int i = 0; i < player_count; i++)
			{
				if (finish[i])
				{
					text_shader.uniform("text_color", player_infos[i].color);

					out_text.str("");

					if (player_positions[i].x > 22)
					{
						out_text << player_infos[i].name << " has voted for level 2";

						float width = build_text.get_text_width(out_text.str(), 1.f);
						build_text.render_text(out_text.str(), screen_width - width, screen_height - (45.f * (level_1_index + 2)), 1.f);

						level_1_index++;
					}
					else if (player_positions[i].x < -22)
					{
						out_text << player_infos[i].name << " has voted for level 1";

						build_text.render_text(out_text.str(), 10.f, screen_height - (45.f * (level_2_index + 2)), 1.f);

						level_2_index++;
					}

					total_players_ready++;
				}
			}

			text_shader.uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));

			if (total_players_ready == player_count)
				build_text.render_text("Host: Press 'Space' to start", screen_width * 0.33f + 120.f, screen_height - 35.f, 0.75f);
		}


		if ((game_state & state::building) && build_info.size() > 0)
		{
			//Timer
			world_text_shader.use();
			world_text_shader.uniform("view", game_camera.view());
			world_text_shader.uniform("projection", game_camera.projection);

			if (print_time > 5.f)
				world_text_shader.uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));
			else
				world_text_shader.uniform("text_color", glm::vec3(0.8f, 0.2f, 0.2f));

			float width = timer_text.get_text_width(out_text.str(), 0.02f);

			timer_text.render_text(out_text.str(), build_info[player_id].local_position.x - (width * 0.5f), build_info[player_id].local_position.y - 0.5f, 0.02f);

			//Build area
			int total_players_ready = 0;
			for (int i = 0; i < player_count; i++)
			{
				lines.use();
				lines.uniform("projection", game_camera.projection);
				lines.uniform("view", game_camera.view());

				if (build_info[i].place_state == 0)	//Cannot Place
				{
					lines.uniform("line_color", glm::vec3(1.0f, 0.0f, 0.0f));
				}
				else if (build_info[i].place_state == 1) //Can Place
				{
					lines.uniform("line_color", glm::vec3(0.2f, 1.0f, 0.2f));
				}
				else if (build_info[i].place_state == 2) //Has Placed
				{
					lines.uniform("line_color", glm::vec3(0.0f, 0.0f, 1.0f));
					total_players_ready++;
				}
				
				point_debug(build_info[i].debug_positions);
			}

			//Other Text
			text_shader.use();
			text_shader.uniform("projection", projection);
			text_shader.uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));

			build_text.render_text("Press 'Space' to place object", screen_width - 540, 10.f, 0.75f);
			build_text.render_text("Build Stage", screen_width - 210, screen_height - 35.f, 0.75f);

			out_text.str("");
			out_text << total_players_ready << "/" << player_count;
			build_text.render_text(out_text.str(), screen_width - 65.f, screen_height - 65.f, 0.75f);


			build_text.render_text("Your object:", 10.f, 45.f, 0.75f);
			build_text.render_text(objects_name[player_object_id], 10.f, 10.f, 0.75f);
		}

		if (game_state & state::pre_playing)
		{

			std::stringstream out_text;

			out_text << std::fixed << std::setprecision(0) << print_time;
			text_shader.use();
			text_shader.uniform("projection", projection);
			text_shader.uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));

			float width = build_text.get_text_width(out_text.str(), 2.f);

			if (print_time > 0.0f)
			{
				build_text.render_text(out_text.str(), (screen_width * 0.5f) - (width * 0.5f), screen_height * 0.45f, 2.f);
			}
		}

		if (game_state & state::playing)
		{


			text_shader.use();
			text_shader.uniform("projection", projection);
			text_shader.uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));

			if (print_time <= 90.f && print_time >= 89.f)
			{
				float width = build_text.get_text_width("GO!", 2.0f);
				build_text.render_text("GO!", (screen_width * 0.5f) - (width * 0.5f), screen_height * 0.45f, 2.f);
			}
			
			if (print_time > 15.f || died[player_id] || finish[player_id])
			{
				text_shader.uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));
				timer_text.render_text(out_text.str(), 10.f, screen_height - 45.f, 1.f);
			}
			else
			{
				static float t = 0.f;
				glm::vec2 start = { 10.f, screen_height - 45.f };
				glm::vec2 end = { (screen_width * 0.5f), (screen_height * 0.7f) };

				glm::vec3 red = glm::vec3(1.f, 0.2f, 0.2f);
				glm::vec3 white = glm::vec3(1.f, 0.4f, 0.4f);

				float start_size = 1.f;
				float end_size = 4.f;

				t = 15 - print_time;

				float cos_t = (glm::cos(t * 4.f) + 1.f) / 2.f;
				glm::vec3 color = red + (white - red) * cos_t;

				if (t > 1.0f)
					t = 1.f;

				glm::vec2 current = start + (end - start) * t;
				float current_size = start_size + (end_size - start_size) * t;
				float width = timer_text.get_text_width(out_text.str(), current_size);

				text_shader.uniform("text_color", color);
				timer_text.render_text(out_text.str(), current.x - (width * 0.5f), current.y, current_size);
			}

			if (finish[player_id] && !died[player_id])
			{
				//Escaped	-- till 5 sekunder
				//Place -- Efter 1.5 sekunder till 5 sekunder
				//Score -- Efter 3 sekunder till 5 sekunder

				float width = 0;

				if (overlays.finished_timer <= 5000ms)
				{
					width = build_text.get_text_width("Escaped", 2.f);
					build_text.render_text("Escaped", (screen_width * 0.5f) - (width * 0.5f), (screen_height * 0.5f) + 35.f, 2.f);
				}

				if (overlays.finished_timer <= 5000ms && overlays.finished_timer >= 1500ms)
				{
					out_text.str("");
					out_text << "Place: " << placing[player_id];
					width = build_text.get_text_width(out_text.str(), 1.25f);
					build_text.render_text(out_text.str(), (screen_width * 0.5f) - (width * 0.5f) + 20.f, (screen_height * 0.5f) - 35.f, 1.25f);
				}

				if (overlays.finished_timer <= 5000ms && overlays.finished_timer >= 3000ms)
				{
					out_text.str("");
					out_text << "Score: +" << scores_to_give[placing[player_id] - 1];
					width = build_text.get_text_width(out_text.str(), 0.75f);
					build_text.render_text(out_text.str(), (screen_width * 0.5f) - (width * 0.5f) + 10.f, (screen_height * 0.5f) - 105.f, 0.75f);
				}
			}

			if (died[player_id])
			{
				out_text.str("");
				out_text << "Respawning at last checkpoint in " << std::fixed << std::setprecision(0) << spawn_timer << " seconds";
				build_text.render_text(out_text.str(), (screen_width * 0.5f) - 400.f, (screen_height * 0.5f) - 70.f, 0.75f);
			}

			if ((!died[player_id] && finish[player_id]) && (overlays.finished_timer >= 5000ms))
			{				
				build_text.render_text("Press 'A' or 'D' to change spectator", (screen_width * 0.5f) - 325.f, screen_height - 35.f, 0.75f);
			}
			else
			{
				minimap.render(minimap_shader);
				gui.use();
				ui.render(gui);
			}

		}

		if (game_state & state::score || view_score)
		{
			text_shader.use();
			text_shader.uniform("projection", projection);
			text_shader.uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));

			build_text.render_text("Score: ", screen_width * 0.5f, screen_height * 0.5f, 0.75f);

			std::array<int, 4> positions;

			for (int i = 0; i < player_count; i++)
			{
				out_text.str("");

				out_text << player_infos[i].name << " : " << player_infos[i].score;

				text_shader.uniform("text_color", player_infos[i].color);

				build_text.render_text(out_text.str(), screen_width * 0.5f, (screen_height * 0.5f) + ((i + 1) * -35.f), 0.75f);
			}
		}

		if (game_state & state::game_over)
		{
			text_shader.use();
			text_shader.uniform("projection", projection);
			text_shader.uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));
			auto width{ .0f }, big_text_size{ 2.5f };
			auto sorted_infos = player_infos;

			std::sort(sorted_infos.begin(), sorted_infos.end(), sort_by_score);
			if (sorted_infos.at(0).score == sorted_infos.at(1).score)
			{
				out_text.str("DRAW");
			}
			else
			{
				out_text.str("");
				text_shader.uniform("text_color", sorted_infos.at(0).color);
				out_text << sorted_infos.at(0).name << " is the winner!";
			}
			width = build_text.get_text_width(out_text.str(), big_text_size);
			build_text.render_text(out_text.str(), screen_width * 0.5f - width * 0.5, screen_height * 0.8f, big_text_size);

			text_shader.uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));
			build_text.render_text("Score", screen_width * 0.46f, screen_height * 0.5f, 0.75f);
			for (auto i = 0u; i < player_count; ++i)
			{
				out_text.str("");
				out_text << sorted_infos.at(i).name << " : " << sorted_infos.at(i).score;
				text_shader.uniform("text_color", sorted_infos.at(i).color);
				build_text.render_text(out_text.str(), screen_width * 0.46f, (screen_height * 0.5f) + ((i + 1) * -35.f), 0.75f);
			}
			text_shader.uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));
			out_text.str("Host: Press SPACE to restart");
			width = build_text.get_text_width(out_text.str(), 0.5f);
			build_text.render_text(out_text.str(), screen_width * 0.5f - width * 0.5, screen_height * 0.1f, 0.5f);
		}

		glEnable(GL_DEPTH_TEST);
	}

	{
		glDisable(GL_DEPTH_TEST);

		text_shader.use();
		glm::mat4 projection = glm::ortho(0.0f, 1920.f, 0.0f, 1080.f);
		text_shader.uniform("projection", projection);
		text_shader.uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));

		auto offset = 0.0f;

		std::for_each(begin, end,
			[this, &offset, begin](const auto& s)
		{
			if (&s == begin || is_chat_visible)
				text.render_text(s.c_str(), 10.f, (offset += 25.0f), 0.5f);
		});

		constexpr float size_y = static_cast<int>(1080 / 12);

		for (auto i = 0u; i < buttons.size(); ++i)
			text.render_text(buttons[i], 10.0f, i * size_y, 1.0f);

		if (player_count > 1)
		{
			//leaderboard
			if (game_over)
			{
				leaderboard.render(text_shader, text);
			}
		}
		
		glEnable(GL_DEPTH_TEST);
	}

	//vramUsage();
	//ramUsage();

}

void Renderer::update(std::chrono::milliseconds delta,
	const objects_array& dynamics,
	const glm::vec2& cursor,
	const std::array<glm::vec3, 4>& directions,
	const std::string& data,
	int num_players,
	int id,
	int new_game_state,
	std::array<bool, 4> died,
	std::array<bool, 4> finish,
	std::array<float, 4> scores,
	std::array<int, 4> trigger_type,
	std::array<int, 4> random_active,
	float print_time,
	float goal_height,
	std::vector<build_information>& all_placed_objects,
	int spectator_id,
	std::array<int, 4> moving_objects_id,
	bool view_score, float dash_timer,
	int turretframe,
	const std::vector<glm::vec2>& start,
	const std::vector<glm::vec2>& end)
{
	bool is_menu = (new_game_state & state::menu);
	float dt = std::chrono::duration_cast<std::chrono::duration<float>>(delta).count();

	s_to_render = ModelsToRender{ game_camera.position.y, &scene->models[9], &scene->models.back() };
	a_to_render = ModelsToRender{ game_camera.position.y, &scene->animated_models.front(), &scene->animated_models.back() };
	
	if (game_state & state::pre_playing || game_state & state::lobby)
	{
		build_stage_screen.timer = 0ms;
		build_stage_screen.transparency = 0.8f;
	}

	if (game_state & state::pre_building)
	{
		places = 1;
		placing = { -1, -1, -1, -1 };
		scores_to_give = { 4, 3, 2, 1 };
	}

	if (!(game_state & state::playing))
	{
		post_processing_effects.glow_value = 0.0f;
		if (!is_menu && game_state & state::pre_building)
		{
			build_stage_screen.timer += delta;
			if (build_stage_screen.timer > 2500ms)
			{
				build_stage_screen.transparency -= 0.015f;
			}
		}
	}
	else if (game_state & state::playing)
	{
		if (print_time <= 15.0f)
			post_processing_effects.update(delta);
		
		for (int i = 0; i < player_count; i++)
		{
			if (!died[i] && finish[i] && placing[i] == -1)
			{
				placing[i] = places;
				places++;
			}
		}
	}

	if (game_state & state::score || view_score || game_state & state::lobby || game_state & state::game_over)
	{
		for (int i = 0; i < player_count; i++)
		{
			player_info info;

			info.my_id = i;
			info.score = scores[i];
			info.name = players[i];
			info.color = players_colors[i];

			player_infos[i] = info;
		}
	}

	//Change to num_players + 1 to see the game loop, without + 1 will show loading screen.
	player_count = num_players;
	game_state = new_game_state;
	player_id = id;
	bool is_chat_on = (game_state & state::chat);

	using namespace std::chrono_literals;

	time = data != log ? 0ms : time + delta;
	log = data;
	is_chat_visible = is_chat_on || time < 3s;

	//--FX Calculations--
	fx_emitter.timer += delta;
	if (!is_chat_on)
	{
		if (scene->level_name == "lobby.ssp")
		{
			current_map = 0;
		}
		else if (scene->level_name == "level_1.ssp")
		{
			current_map = 1;
		}
		else if (scene->level_name == "level_2.ssp")
		{
			current_map = 2;
		}
		else
		{
			current_map = -1;
		}

		if (current_map == 1)
		{
			//Spark
			fx_emitter.calculate_spark_data(delta, game_camera);

			//Blitz
			fx_emitter.calculate_blitz_data(delta, game_camera);

			//Fire
			fx_emitter.calculate_fire_data(delta, game_camera);

			//Gust
			fx_emitter.calculate_gust_data(delta, game_camera);
		}

		//Dust
		fx_emitter.calculate_dust_data(delta, game_camera);

		//Bubble
		fx_emitter.calculate_bubble_data(delta, game_camera);

		//Steam
		fx_emitter.calculate_steam_data(delta, game_camera, current_map);
		
		//Godray
		fx_emitter.calculate_godray_data(delta, game_camera, current_map);
		
		//Lava Light
		fx_emitter.calculate_lava_light_data(delta, game_camera, current_map);
		
		//Furnace Light
		fx_emitter.calculate_furnace_light_data(delta, game_camera, current_map);

		//Objects
		fx_emitter.calculate_object_data(delta, game_camera, all_placed_objects, trigger_type[player_id], random_active[player_id], died[player_id], game_state, dynamics[player_id].bullet_hit, scene->moving_models[player_id].get_position());

		db_camera.update(delta, directions[0], cursor);
		ui.disable_chat();
	}
	else
	{
		ui.enable_chat();
	}

	for (auto i = 0u; i < num_players; i++)
	{
		ui.player_arrows.set_visibility(i, (died[i] || finish[i]));
	}
	
	if (!died[id] && !finish[id])
		game_camera.update(delta, &scene->v[id], &scene->v[id + 1]);
	else
		game_camera.update(delta, &scene->v[spectator_id], &scene->v[spectator_id + 1]);

	if (died[player_id])
	{
		spawn_timer -= dt;
	}
	else
		spawn_timer = 3.5f;

	ui.update(scene->moving_models, 
		player_count, 
		game_camera.position);

	minimap.update(scene->moving_models, player_count, goal_height);

	for (int i = 0; i < 4; ++i)
	{
		scene->lights[i].position = scene->moving_models[i].get_position();
	}

	anim idle = anim::falling;
	for (auto it = a_to_render.first; it != a_to_render.last; ++it)
		it->update_animation((float)delta.count(), idle);

	laser.update(turretframe, start, end);
	
	grid.update(game_camera, scene->lights);

	overlays.update(delta, 
		died[player_id], 
		finish[player_id],
		scores,
		trigger_type[player_id],
		game_state, 
		player_id,
		dash_timer);
}

void Renderer::update_moving_platforms(const objects_array& dynamics,
	int model_id,
	int nr_of_moving_platforms)
{	
	if (scene->moving_models.size() >= 4)
	{
		int j = 0;
		for (int i = model_id; i < model_id + nr_of_moving_platforms; i++)
		{
			scene->moving_models[i].set_position(dynamics[j].position);
			j++;
		}
	}
}

void Renderer::render_type(const Shader& shader, const Camera& camera, const Model* first, const Model* last) const
{
	shader.use();
	
	shader.uniform("view", camera.view());
	shader.uniform("projection", camera.projection);

	shader.uniform("cam_pos", camera.position);
	shader.uniform("dir_light_dir", dir_light.direction);
	shader.uniform("dir_light_color", dir_light.color);
	shader.uniform("dir_light_intensity", dir_light.intensity);
	shader.uniform("light_count", (int)scene->lights.size());

	int light_count = 0;

	for (int i = 0; i < scene->lights.size(); ++i)
	{
		shader.uniform("light_pos[" + std::to_string(i) + "]", 
			scene->lights[i].position);
		shader.uniform("light_color[" + std::to_string(i) + "]", 
			scene->lights[i].color);
		shader.uniform("light_intensity[" + std::to_string(i) + "]", 
			scene->lights[i].intensity);
	}

	for (auto it = first; it != last; ++it)
	{
		const auto& renderable = *it;
		renderable.render(shader);
	}

}

void Renderer::render_character(const Shader& shader, const Camera& camera, const std::vector<Model>& data, int num_players) const
{
	shader.use();

	shader.uniform("view", camera.view());
	shader.uniform("projection", camera.projection);

	shader.uniform("cam_pos", camera.position);
	shader.uniform("light_count", (int)scene->lights.size());
	shader.uniform("dir_light_dir", dir_light.direction);
	shader.uniform("dir_light_color", dir_light.color);
	shader.uniform("dir_light_intensity", dir_light.intensity);

	int light_count = 0;

	for (int i = 4; i < scene->lights.size(); ++i)
	{
		shader.uniform("light_pos[" + std::to_string(i) + "]",
			scene->lights[i].position);
		shader.uniform("light_color[" + std::to_string(i) + "]",
			scene->lights[i].color);
		shader.uniform("light_intensity[" + std::to_string(i) + "]",
			scene->lights[i].intensity);
	}
	
	for (auto i = 0; i < num_players; ++i)
	{
		const auto& renderable = data[i];
		renderable.render(shader);
	}
}
}