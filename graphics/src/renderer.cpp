#include "renderer.hpp"

#include <iostream>
#include <algorithm>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>

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

	for (int i = 0; i < 4; ++i)
	{
		lights[i].position = glm::vec3{0,0,0};
		lights[i].color = glm::vec3{1,1,1};
	}

}

void Renderer::render(
	const std::string* begin,
	const std::string* end,
	const std::array<std::string, 12>& buttons,
	const std::vector<glm::vec3>& debug_positions)const
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
		pbr.use();
		pbr.uniform("brdf_lut", 6);
		pbr.uniform("irradiance_map", 7);
		pbr.uniform("prefilter_map", 8);

		brdf_buffer.bind_texture(6);
		irradiance_buffer.bind_texture(7);
		prefilter_buffer.bind_texture(8);

		render_character(pbr, 
			game_camera, lights, scene->models, player_count);
		render_type(pbr, game_camera, lights, scene->models);

		skybox_shader.use();
		skybox.render(skybox_shader, game_camera);

		fx_emitter.render_particles(fx_dust, fx_spark, fx_steam, game_camera);

		glDisable(GL_DEPTH_TEST);
		if (debug_active)
		{
			lines.use();
			lines.uniform("projection", game_camera.projection);
			lines.uniform("view", game_camera.view());
			lines.uniform("line_color", glm::vec3(0.2, 1.0, 0.2f));
			line_debug(debug_positions);
			glEnable(GL_DEPTH_TEST);
		}

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
			render_character(pbr, 
				db_camera, lights, scene->models, 4);
		render_type(pbr, db_camera, lights, scene->models);

		skybox_shader.use();
		skybox.render(skybox_shader, db_camera);

		fx_emitter.render_particles(fx_dust, fx_spark, fx_steam, game_camera);

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

	post_proccessing.use();
	post_proccessing.uniform("scene_texture", 0);
	post_proccessing.uniform("depth_texture", 1);
	post_proccessing.uniform("screen_warning", 2);

	scene_texture.bind_texture(0);
	scene_texture.bind_texture(1);
	post_processing_effects.texture.bind(2);

	post_proccessing.uniform("pulse", post_processing_effects.glow_value);
	post_processing_effects.render();

	{
		glDisable(GL_DEPTH_TEST);

		// Text
		gui.use();
		if (is_chat_visible)
		{
			ui.render();
		}

		//leaderboard
		leaderboard.render(text_shader);

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

		if (!is_menu)
			minimap.render(minimap_shader);


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
	std::string scoreboard)
{
	player_count = num_players;
	game_state = new_game_state;
	bool is_chat_on = (game_state & state::chat);
	
	using namespace std::chrono_literals;	
	
	time = data != log ? 0ms : time + delta;
	log = data;
	is_chat_visible = is_chat_on || time < 3s;

	if (!is_chat_on)
	{
		//Dust Particles
		fx_emitter.calculate_dust_data(delta, db_camera);
		//dust_particles->calculate_dust_data(*dust_particles->fx, scene->v, delta, db_camera);

		//Steam Particles
		//steam_particles->calculate_steam_data(*steam_particles->fx, scene->v, delta, db_camera);
		fx_emitter.calculate_steam_data(delta, db_camera);

		db_camera.update(delta, directions[0], cursor);
	}

	if (scene->build_mode_active)
	{
		glm::vec2 build_pos[2];

		//game_camera.update(delta, &scene->v[scene->placing_object_id], &scene->v[scene->placing_object_id + 1]);
	}

	game_camera.update(delta, &scene->v[id], &scene->v[id + 1]);
	ui.update();
	minimap.update(scene->models, player_count);

	for (int i = 0; i < 4; ++i)
	{
		lights[i].position = scene->models[i].get_position();
	}

	leaderboard.update(std::move(scoreboard));

}

}