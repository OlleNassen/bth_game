#include "renderer.hpp"

#include <iostream>
#include <algorithm>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include <iomanip>

namespace graphics
{

using namespace std::chrono_literals;

ModelsToRender::ModelsToRender(const Model& player, const Model* begin, const Model* end)
{
	first = begin;
	last = end - 1;

	float culling_distance = 50.0f;
	float bottom = player.get_y_position() - culling_distance;
	float top = player.get_y_position() + culling_distance;

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
	, irradiance_buffer{irradiance, skybox}
	, prefilter_buffer{pre_filter, skybox, true}
	, brdf_buffer{brdf, skybox, 3.f}
	, leaderboard(projection)
{

	db_camera.position.z = 20.0f;
	glViewport(0, 0, 1920, 1080); // don't forget to configure the viewport to the capture dimensions.
	
	//Player Light
	lights[0].color = glm::vec3{ 9.0f, 1.0f, 1.0f };
	lights[0].intensity = 30;
	lights[1].color = glm::vec3{ 0.2f, 0.9f, 0.1f };
	lights[1].intensity = 30;
	lights[2].color = glm::vec3{ 0.1f, 0.1f, 0.9f };
	lights[2].intensity = 30;
	lights[3].color = glm::vec3{ 0.9f, 0.8f, 0.1f };
	lights[3].intensity = 30;

	//Map Light
	lights[4].position = glm::vec3{ -0.145,-6.289,8.929 };
	lights[4].color = glm::vec3{ 1,0.2,0 };
	lights[5].position = glm::vec3{ -7.73,23.368,-22.735 };
	lights[5].color = glm::vec3{ 1,0.2,0 };
	lights[6].position = glm::vec3{ -7.74,44,-22.984 };
	lights[6].color = glm::vec3{ 1,0.2,0 };
	lights[7].position = glm::vec3{ 0, 123.035, -23.725 };
	lights[7].color = glm::vec3{ 0, 0.82, 1 };
	lights[8].position = glm::vec3{ 0, 187.484, -18.185 };
	lights[8].color = glm::vec3{ 1, 0.2, 0 };
	lights[9].position = glm::vec3{ 0, 208.400, -34.226 };
	lights[9].color = glm::vec3{ 1, 0.2, 0 };
	lights[10].position = glm::vec3{ 0, 260.169, 5.0 };
	lights[10].color = glm::vec3{ 1, 1, 1 };
	lights[11].position = glm::vec3{ 0, 88.553, -29.053 };
	lights[11].color = glm::vec3{ 1, 0.2, 0 };
	lights[12].position = glm::vec3{ 0, 135.246, -11.474 };
	lights[12].color = glm::vec3{ 1, 0.2, 0 };

	for (int i = 4; i < 13; i++)
	{
		lights[i].intensity = 400;
	}

	lights[5].intensity = 700;
	lights[6].intensity = 1000;

	dir_light.direction = glm::vec3(0, -0.7, -1);
	dir_light.color = glm::vec3(1.0, 0.8, 0.8);
	dir_light.intensity = 0.4f;

	//spotlights[0].position = glm::vec3(0, 30, 0);
	//spotlights[0].color = glm::vec3(1.f, 1.0f, 0.0f);
	//spotlights[0].direction = glm::vec3(0, -1, 0);
	//spotlights[0].intensity = 200.f;
	//spotlights[0].cos_total_width = std::cos(glm::radians(10.f));
	//spotlights[0].cos_falloff_start = std::cos(glm::radians(8.f));
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
	float print_time,
	int player_id,
	int player_object_id,
	std::vector<glm::vec3> remove_lines)const
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

	if (!is_menu && connected)
	{
		render_character(robot_shader, game_camera, scene->moving_models, player_count); 

		if (scene->moving_models.size() > 4)
			render_type(pbra, game_camera, &scene->moving_models[4], &scene->moving_models.back() + 1);

		render_type(pbra, game_camera,a_to_render.first, a_to_render.last);
		render_type(pbr, game_camera, s_to_render.first, s_to_render.last);
		render_type(pbr, game_camera,&scene->models[0], &scene->models[9]);
		
		if (!(game_state & state::lobby))
		{
			fx_emitter.render_particles(fx_dust, fx_spark, fx_steam, fx_blitz, fx_fire, fx_godray, fx_gust, fx_stun, game_camera, fx_emitter.timer);
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
	else if (!is_menu)
	{
		if(debug)
			render_character(robot_shader, game_camera, scene->moving_models, 4);

		if (scene->moving_models.size() > 4)
			render_type(pbr, db_camera,
				&scene->moving_models[4], &scene->moving_models.back() + 1);

		render_type(pbra, db_camera, a_to_render.first, a_to_render.last);
		render_type(pbr, db_camera, s_to_render.first, s_to_render.last);
		render_type(pbr, db_camera, &scene->models[0], &scene->models[9]);

		if (!(game_state & state::lobby))
		{
			fx_emitter.render_particles(fx_dust, fx_spark, fx_steam, fx_blitz, fx_fire, fx_godray, fx_gust, fx_stun, game_camera, fx_emitter.timer);
		}

		if (debug_active)
		{
			glDisable(GL_DEPTH_TEST);
			lines.use();
			lines.uniform("projection", db_camera.projection);
			lines.uniform("view", db_camera.view());
			lines.uniform("line_color", glm::vec3(0.2, 1.0, 0.2f));
			point_debug(debug_positions);
			glEnable(GL_DEPTH_TEST);
		}

	}

	// Post Processing Effects
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	overlays.render(overlay_shader);

	//Text rendering
	if (!is_menu)
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

		if (game_state & state::lobby)
		{
			text_shader.use();
			text_shader.uniform("projection", projection);
			text_shader.uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));
			build_text.render_text("Lobby Stage, Host press 'R' to start", screen_width * 0.33f, screen_height - 35.f, 0.75f);

			if (scores[0] > 0)
			{
				build_text.render_text("Score: ", screen_width * 0.5f, screen_height * 0.5f, 0.75f);

				for (int i = 0; i < player_count; i++)
				{
					out_text.str("");
					out_text << players[i] << " : " << scores[i];
					text_shader.uniform("text_color", players_colors[i]);
					build_text.render_text(out_text.str(), screen_width * 0.5f, (screen_height * 0.5f) + ((i + 1) * -35.f), 0.75f);
				}
			}
		}

		if (game_state & state::pre_building)
		{
			
		}

		if (game_state & state::building)
		{
			//Other Text
			text_shader.use();
			text_shader.uniform("projection", projection);
			text_shader.uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));

			build_text.render_text("Press 'Space' to place object", screen_width - 540, 10.f, 0.75f);
			build_text.render_text("Build Stage", screen_width - 210, screen_height - 35.f, 0.75f);
			build_text.render_text("Your object:", 10.f, 45.f, 0.75f);
			build_text.render_text(objects_description[player_object_id], 10.f, 10.f, 0.75f);

			//Timer
			world_text_shader.use();
			world_text_shader.uniform("view", game_camera.view());
			world_text_shader.uniform("projection", game_camera.projection);

			if (print_time > 5.f)
				world_text_shader.uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));
			else
				world_text_shader.uniform("text_color", glm::vec3(0.8f, 0.2f, 0.2f));

			float width = timer_text.get_text_width(out_text.str(), 0.02f);

			timer_text.render_text(out_text.str(), build_info[player_id].local_position.x - (width * 0.5f), build_info[player_id].local_position.y + 1.f, 0.02f);

			//Build area
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
				}

				point_debug(build_info[i].debug_positions);
			}
		}

		if (game_state & state::pre_playing)
		{
			std::stringstream out_text;

			/*if (print_time <= 1.0f)
			{
				print_time = 1.0f;
			}*/

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

			if ((died[player_id] || finish[player_id]) && (overlays.finished_timer <= 5000ms || overlays.death_timer >= 2000ms))
			{				
				build_text.render_text("Press 'A' or 'D' to change spectator", (screen_width * 0.5f) - 325.f, screen_height - 35.f, 0.75f);
			}
		}

		if (game_state & state::score)
		{
			text_shader.use();
			text_shader.uniform("projection", projection);
			text_shader.uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));

			build_text.render_text("Score: ", screen_width * 0.5f, screen_height * 0.5f, 0.75f);

			for (int i = 0; i < player_count; i++)
			{
				out_text.str("");
				out_text << players[i] << " : " << scores[i];
				text_shader.uniform("text_color", players_colors[i]);
				build_text.render_text(out_text.str(), screen_width * 0.5f, (screen_height * 0.5f) + ((i + 1) * -35.f), 0.75f);
			}
		}

		if (game_state & state::game_over)
		{

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
				text.render_text(s.c_str(), 10.0f, (offset += 25.0f), 0.5f);
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

			if (!is_menu && !finish[player_id] && !died[player_id])
			{
				minimap.render(minimap_shader);
				gui.use();

				ui.render(gui);
			}
		}

		glEnable(GL_DEPTH_TEST);
	}

}

void Renderer::update(std::chrono::milliseconds delta,
	const objects_array& dynamics,
	const glm::vec2& cursor,
	const std::array<glm::vec3, 4>& directions,
	const std::string& data,
	int num_players,
	int id,
	int new_game_state,
	std::string scoreboard,
	std::array<bool, 4> died,
	std::array<bool, 4> finish,
	std::array<float, 4> scores,
	float print_time,
	float goal_height,
	std::vector<build_information>& all_placed_objects,
	int spectator_id,
	std::array<int, 4> moving_objects_id)
{
	bool is_menu = (new_game_state & state::menu);
	
	if (game_state & state::building)
	{
		s_to_render = ModelsToRender{ scene->moving_models[moving_objects_id[id]], &scene->models[9], &scene->models.back() };
		if (scene->animated_models.size() > 0)
			a_to_render = ModelsToRender{ scene->moving_models[moving_objects_id[id]], &scene->animated_models.front(), &scene->animated_models.back() };
	}
	else if (!died[id] && !finish[id])
	{
		s_to_render = ModelsToRender{ scene->moving_models[id], &scene->models[9], &scene->models.back() };
		if (scene->animated_models.size() > 0)
			a_to_render = ModelsToRender{ scene->moving_models[id], &scene->animated_models.front(), &scene->animated_models.back() };
	}
	else
	{
		s_to_render = ModelsToRender{ scene->moving_models[spectator_id], &scene->models[9], &scene->models.back() };
		if (scene->animated_models.size() > 0)
			a_to_render = ModelsToRender{ scene->moving_models[spectator_id], &scene->animated_models.front(), &scene->animated_models.back() };
	}
	
	if (game_state & state::score)
	{
		build_stage_screen.timer = 0ms;
		build_stage_screen.transparency = 1.0f;
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
	else if (game_state & state::playing && print_time <= 15.0f)
	{
		post_processing_effects.update(delta);
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
		//Dust
		fx_emitter.calculate_dust_data(delta, game_camera);

		//Spark
		fx_emitter.calculate_spark_data(delta, game_camera);

		//Steam
		fx_emitter.calculate_steam_data(delta, game_camera);

		//Blitz
		fx_emitter.calculate_blitz_data(delta, game_camera);

		//Fire
		fx_emitter.calculate_fire_data(delta, game_camera);

		//Godray
		fx_emitter.calculate_godray_data(delta, game_camera);
 
		//Lava Light
		fx_emitter.calculate_lava_light_data(delta, game_camera);

		//Furnace Light
		fx_emitter.calculate_furnace_light_data(delta, game_camera);

		//Gust
		fx_emitter.calculate_gust_data(delta, game_camera);

		
		fx_emitter.calculate_object_data(delta, game_camera, all_placed_objects);
		

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

	ui.update(scene->moving_models, 
		player_count, 
		game_camera.position);

	minimap.update(scene->moving_models, player_count, goal_height);

	for (int i = 0; i < 4; ++i)
	{
		lights[i].position = scene->moving_models[i].get_position();
	}

	leaderboard.update(std::move(scoreboard));

	overlays.update(delta, 
		died[player_id], 
		finish[player_id], 
		scores, 
		game_state, 
		player_id);
}

void Renderer::render_type(const Shader& shader, const Camera& camera, const Model* first, const Model* last) const
{
	shader.use();

	shader.uniform("brdf_lut", 6);
	shader.uniform("irradiance_map", 7);
	shader.uniform("prefilter_map", 8);
	brdf_buffer.bind_texture(6);
	irradiance_buffer.bind_texture(7);
	prefilter_buffer.bind_texture(8);

	shader.uniform("view", camera.view());
	shader.uniform("projection", camera.projection);

	shader.uniform("cam_pos", camera.position);
	shader.uniform("dir_light_dir", dir_light.direction);
	shader.uniform("dir_light_color", dir_light.color);
	shader.uniform("dir_light_intensity", dir_light.intensity);

	shader.uniform("spotlight_pos", spotlights[0].position);
	shader.uniform("spotlight_color", spotlights[0].color);
	shader.uniform("spotlight_direction", spotlights[0].direction);
	shader.uniform("spotlight_intensity", spotlights[0].intensity);
	shader.uniform("cos_total_width", spotlights[0].cos_total_width);
	shader.uniform("cos_falloff_start", spotlights[0].cos_falloff_start);

	int light_count = 0;

	for (int i = 0; i < 13; i++)
	{
		if (abs(lights[i].position.y - camera.position.y) < 80.0f)
		{
			shader.uniform("light_pos[" + std::to_string(light_count) + "]", lights[i].position);
			shader.uniform("light_color[" + std::to_string(light_count) + "]", lights[i].color);
			shader.uniform("light_intensity[" + std::to_string(light_count) + "]", lights[i].intensity);
			light_count++;
		}
	}

	shader.uniform("light_count", light_count);

	for (auto it = first; it != last; ++it)
	{
		const auto& renderable = *it;
		renderable.render(shader);
	}
}

void Renderer::render_character(const Shader& shader, const Camera& camera, const std::vector<Model>& data, int num_players) const
{
	shader.use();

	shader.uniform("brdf_lut", 6);
	shader.uniform("irradiance_map", 7);
	shader.uniform("prefilter_map", 8);
	brdf_buffer.bind_texture(6);
	irradiance_buffer.bind_texture(7);
	prefilter_buffer.bind_texture(8);

	shader.uniform("view", camera.view());
	shader.uniform("projection", camera.projection);
	shader.uniform("cam_pos", camera.position);
	shader.uniform("dir_light_dir", dir_light.direction);
	shader.uniform("dir_light_color", dir_light.color);
	shader.uniform("dir_light_intensity", dir_light.intensity);

	int light_count = 0;

	for (int i = 0; i < 13; i++)
	{
		if (abs(lights[i].position.y - camera.position.y) < 80.0f)
		{
			shader.uniform("light_pos[" + std::to_string(light_count) + "]", lights[i].position);
			shader.uniform("light_color[" + std::to_string(light_count) + "]", lights[i].color);
			shader.uniform("light_intensity[" + std::to_string(light_count) + "]", lights[i].intensity);
			light_count++;
		}
	}

	shader.uniform("light_count", light_count);


	for (auto i = 0; i < num_players; ++i)
	{
		const auto& renderable = data[i];
		renderable.render(shader);
	}
}

}