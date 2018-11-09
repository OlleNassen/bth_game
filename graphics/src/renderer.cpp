#include "renderer.hpp"

#include <iostream>
#include <algorithm>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include <iomanip>

namespace graphics
{

using namespace std::chrono_literals;



Renderer::Renderer(GameScene* scene)
	: db_camera{glm::radians(90.0f), 1280.f / 720.f, 0.1f, 100.f}
	, game_camera{glm::radians(65.0f), 1280.f / 720.f, 0.1f, 100.f}
	, scene{scene}
	, irradiance_buffer{irradiance, skybox}
	, prefilter_buffer{pre_filter, skybox, true}
	, brdf_buffer{brdf, skybox, 3.f}
	, leaderboard(projection)
{

	db_camera.position.z = 20.0f;
	glViewport(0, 0, 1280, 720); // don't forget to configure the viewport to the capture dimensions.
	
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
	lights[4].position = glm::vec3{ -5.625,0,-20 };
	lights[4].color = glm::vec3{ 1,0.38,0 };
	lights[5].position = glm::vec3{ -5.32,40,-20 };
	lights[5].color = glm::vec3{ 0,0.82,1 };
	lights[6].position = glm::vec3{ 15.821,80,-20 };
	lights[6].color = glm::vec3{ 1,0.48,0 };
	lights[7].position = glm::vec3{ -11.853,120,-20 };
	lights[7].color = glm::vec3{ 0,0.82,1 };
	lights[8].position = glm::vec3{ 13.34,160,-20 };
	lights[8].color = glm::vec3{ 1,0.48,0 };
	lights[9].position = glm::vec3{ -11.853,200,-20 };
	lights[9].color = glm::vec3{ 0,0.82,1 };
	lights[10].position = glm::vec3{ 13.34,240,-20 };
	lights[10].color = glm::vec3{ 1,0.48,0 };

	for (int i = 4; i < 12; i++)
	{
		lights[i].intensity = 200;
	}

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
	float print_time)const
{
	bool is_menu = (game_state & state::menu);
	bool connected = (game_state & state::connected);
	bool debug_active = (game_state & state::render_physics);
	
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene_texture.bind_framebuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!is_menu && connected)
	{
		robot_shader.use();
		robot_shader.uniform("brdf_lut", 6);
		robot_shader.uniform("irradiance_map", 7);
		robot_shader.uniform("prefilter_map", 8);

		brdf_buffer.bind_texture(6);
		irradiance_buffer.bind_texture(7);
		prefilter_buffer.bind_texture(8);
		render_character(robot_shader,
			game_camera, lights, scene->moving_models, player_count);   

		pbr.use();
		pbr.uniform("brdf_lut", 6);
		pbr.uniform("irradiance_map", 7);
		pbr.uniform("prefilter_map", 8);

		brdf_buffer.bind_texture(6);
		irradiance_buffer.bind_texture(7);
		prefilter_buffer.bind_texture(8);
		render_type(pbr, game_camera, lights, 
			&scene->models[first_model], &scene->models[last_model]);

		if (scene->moving_models.size() > 4)
			render_type(pbr, game_camera, lights,
				&scene->moving_models[4], &scene->moving_models.back() + 1);

		render_type(pbr, game_camera, lights,
			&scene->models[0], &scene->models[9]);

		skybox_shader.use();
		skybox.render(skybox_shader, game_camera);

		fx_emitter.render_particles(fx_dust, fx_spark, fx_steam, fx_blitz, fx_fire, game_camera);

		glDisable(GL_DEPTH_TEST);
		if (debug_active)
		{
			lines.use();
			lines.uniform("projection", game_camera.projection);
			lines.uniform("view", game_camera.view());
			lines.uniform("line_color", glm::vec3(0.2f, 1.0f, 0.2f));
			line_debug(debug_positions);
			glEnable(GL_DEPTH_TEST);
		}

		/*if (game_state & state::waiting)
		{
			glDisable(GL_DEPTH_TEST);
			text_shader.use();
			text_shader.uniform("projection", projection);
			text_shader.uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));
			build_text.render_text("Press 'Space' to start", 1280.f - 410, 10.f, 0.75f);
			glEnable(GL_DEPTH_TEST);
		}
		else */
		if (game_state & state::building)
		{

			int max = build_info.size();
			for (int i = 0; i < max; i++)
			{
				glDisable(GL_DEPTH_TEST);
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
				else if(build_info[i].place_state == 2)	//Has Placed
				{
					lines.uniform("line_color", glm::vec3(0.0f, 0.0f, 1.0f));
				}

				line_debug(build_info[i].build_positions);
				glEnable(GL_DEPTH_TEST);
			}

			glDisable(GL_DEPTH_TEST);			
			text_shader.use();
			text_shader.uniform("projection", projection);
			text_shader.uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));
			build_text.render_text("Press 'Space' to place object", 1280.f - 540, 10.f, 0.75f);
			glEnable(GL_DEPTH_TEST);

			glDisable(GL_DEPTH_TEST);
			text_shader.use();
			text_shader.uniform("projection", projection);
			text_shader.uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));
			build_text.render_text("Build Stage", 1280.f - 210, 720.f - 35.f, 0.75f);
			glEnable(GL_DEPTH_TEST);
		}

		glDisable(GL_DEPTH_TEST);
		std::stringstream out_text;
		out_text << std::fixed << std::setprecision(1) << print_time;
		text_shader.use();
		text_shader.uniform("projection", projection);
		text_shader.uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));
		timer_text.render_text(out_text.str(), 10.f, 720.f - 45.f, 1.f);
		glEnable(GL_DEPTH_TEST);
	}
	else if (!is_menu)
	{
		pbr.use();
		pbr.uniform("brdf_lut", 6);
		pbr.uniform("irradiance_map", 7);
		pbr.uniform("prefilter_map", 8);

		brdf_buffer.bind_texture(6);
		irradiance_buffer.bind_texture(7);
		prefilter_buffer.bind_texture(8);

		if(debug)
			render_character(robot_shader,
				game_camera, lights, scene->moving_models, 4);

		if (scene->moving_models.size() > 4)
			render_type(pbr, db_camera, lights,
				&scene->moving_models[4], &scene->moving_models.back() + 1);

		render_type(pbr, db_camera, lights, 
			&scene->models[first_model], &scene->models[last_model]);

		render_type(pbr, db_camera, lights,
			&scene->models[0], &scene->models[9]);

		skybox_shader.use();
		skybox.render(skybox_shader, db_camera);

		fx_emitter.render_particles(fx_dust, fx_spark, fx_steam, fx_blitz, fx_fire, game_camera);

		if (debug_active)
		{
			glDisable(GL_DEPTH_TEST);
			lines.use();
			lines.uniform("projection", db_camera.projection);
			lines.uniform("view", db_camera.view());
			lines.uniform("line_color", glm::vec3(0.2, 1.0, 0.2f));
			line_debug(debug_positions);
			glEnable(GL_DEPTH_TEST);
		}

	}

	// Post Processing Effects
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (player_count > 0)
	{
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



			// RENDER BUILD-STAGE
			//if (!build_stage_screen.transparency < 0.05)
	



			glDisable(GL_DEPTH_TEST);

			if (finish[player_id] && died[player_id])
			{
				death_screen.render(overlay_shader);
			}
			if (finish[player_id] && !died[player_id])
			{
				finish_screen.render(overlay_shader, finish);
			}
			if (!build_stage_screen.transparency < 0.0005f)
			{
				build_stage_screen.render(build_stage_screen_shader);
			}
			glEnable(GL_DEPTH_TEST);
		}
	}
	else
	{
		if (!is_menu)
		{
			loading_screen.render(overlay_shader);
		}
	}

	{
		glDisable(GL_DEPTH_TEST);

		if (is_menu)
		{
			main_menu_screen.render(overlay_shader);
		}


		text_shader.use();
		glm::mat4 projection = glm::ortho(0.0f, 1280.f, 0.0f, 720.f);
		text_shader.uniform("projection", projection);
		text_shader.uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));

		auto offset = 0.0f;

		std::for_each(begin, end,
			[this, &offset, begin](const auto& s)
		{
			if (&s == begin || is_chat_visible)
				text.render_text(s.c_str(), 10.0f, (offset += 25.0f), 0.5f);
		});

		constexpr float size_y = static_cast<int>(720 / 12);

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
	float goal_height)

{
	bool is_menu = (game_state & state::menu);
	first_model = 9;
	last_model = 9;
	for (auto i = 9u; i < scene->models.size(); ++i)
	{
		float culling_distance = 50.0f;

		auto bottom = 0.0f; // scene->moving_models[id].get_y_position() - culling_distance;
		auto top = 0.0f; // scene->moving_models[id].get_y_position() + culling_distance;

		if (game_state & state::building)
		{
			bottom = scene->v[id].y - culling_distance;
			top = scene->v[id].y + culling_distance;
		}
		else
		{
			bottom = scene->moving_models[id].get_y_position() - culling_distance;
			top = scene->moving_models[id].get_y_position() + culling_distance;
		}		
		
		if (bottom < scene->models[i].get_y_position() && !first_model)
		{
			first_model = i;
			last_model = i;
		}

		if (top < scene->models[i].get_y_position())
		{
			last_model = i;
			break;
		}

		if (top >= scene->models.back().get_y_position())
		{
			last_model = scene->models.size() - 1;
		}
	}
	
	if (game_state & state::building)
	{
		post_processing_effects.glow_value = 0.0f;
		if (!is_menu)
		{
			build_stage_screen.timer += delta;
			if (build_stage_screen.timer > 2500ms)
			{
				build_stage_screen.transparency -= 0.03f;
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

	//Death screen update
	if (died[id] && finish[id])
	{
		death_screen.timer += delta;
	}
	else
	{
		death_screen.timer = 0ms;
	}

	//Loading screen update
	if (loading_screen.timer > 4000ms)
	{
		loading_screen.timer = 0ms;
	}
	else
	{
		loading_screen.timer += delta;
	}

	//Main menu update
	if (main_menu_screen.timer > 1600ms)
	{
		main_menu_screen.timer = 0ms;
	}
	else
	{
		main_menu_screen.timer += delta;
	}

	//Finish screen update
	if (finish[id] && !died[id])
	{
		finish_screen.timer += delta;
	}
	else
	{
		finish_screen.timer = 0ms;
	}

	if (!is_chat_on)
	{
		//Dust Particles
		fx_emitter.calculate_dust_data(delta, game_camera);

		//Spark Particles
		fx_emitter.calculate_spark_data(delta, game_camera);

		//Steam Particles
		fx_emitter.calculate_steam_data(delta, game_camera);

		//Blitz Particles
		fx_emitter.calculate_blitz_data(delta, game_camera);

		//Fire Particles
		fx_emitter.calculate_fire_data(delta, game_camera);

		db_camera.update(delta, directions[0], cursor);
		ui.disable_chat();
	}
	else
	{
		ui.enable_chat();
	}

	game_camera.update(delta, &scene->v[id], &scene->v[id + 1]);
	ui.update(scene->moving_models, 
		player_count, 
		game_camera.position, 
		died);

	minimap.update(scene->moving_models, player_count, goal_height);

	for (int i = 0; i < 4; ++i)
	{
		lights[i].position = scene->moving_models[i].get_position();
	}

	leaderboard.update(std::move(scoreboard));

}

void Renderer::z_prepass(const std::string * begin, const std::string * end,
	const std::array<std::string, 12>& buttons, const std::vector<glm::vec3>& debug_positions,
	const std::vector<build_information>& build_infos, bool game_over, std::array<bool, 4> died,
	std::array<bool, 4> finish, std::array<float, 4> scores, float print_time) const
{
	//CLEAR FOR Z-PREPASS
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// z-prepass
	glEnable(GL_DEPTH_TEST);  // We want depth test !
	glDepthFunc(GL_LESS);     // We want to get the nearest pixels
	glColorMask(0, 0, 0, 0);  // Disable color, it's useless, we only want depth.
	glDepthMask(GL_TRUE);     // Ask z writing

	render(begin, end, buttons, debug_positions, build_infos, game_over, died, finish, scores, print_time);

	glEnable(GL_DEPTH_TEST);  // We still want depth test
	glDepthFunc(GL_LEQUAL);   // EQUAL should work, too. (Only draw pixels if they are the closest ones)
	glColorMask(1, 1, 1, 1);     // We want color this time
	glDepthMask(GL_TRUE);    // Writing the z component is useless now, we already have it
}

}