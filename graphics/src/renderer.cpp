#include "renderer.hpp"

#include <iostream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace graphics
{

using namespace std::chrono_literals;

Renderer::Renderer(GameScene* scene)
	: db_camera(glm::radians(90.0f), 1280.f / 720.f, 0.1f, 100.f)
	, game_camera(glm::radians(65.0f), 1280.f / 720.f, 0.1f, 100.f)
	, scene { scene }
{
	shaders.reserve(sizeof(Shader) * 10);
	shaders.emplace_back(
		"../resources/shaders/pbr.vs",
		"../resources/shaders/pbr.fs");
	shaders.emplace_back(
		"../resources/shaders/text.vs",
		"../resources/shaders/text.fs");
	shaders.emplace_back(
		"../resources/shaders/gui.vs",
		"../resources/shaders/gui.fs");
	shaders.emplace_back(
		"../resources/shaders/post_processing_effects.vs",
		"../resources/shaders/post_processing_effects.fs");
	shaders.emplace_back(
		"../resources/shaders/temp.vs",
		"../resources/shaders/temp.fs");
	shaders.emplace_back(
		"../resources/shaders/lines.vs",
		"../resources/shaders/lines.fs");
	shaders.emplace_back(
		"../resources/shaders/ibl.vs",
		"../resources/shaders/ibl.fs");

	db_camera.position.z = 20.0f;
}


void Renderer::render(
	const std::string* begin,
	const std::string* end,
	const std::array<std::string, 12>& buttons,
	const std::vector<glm::vec2>& debug_positions,
	bool is_menu,
	bool connected,
	bool debug)const
{
	glClearColor(1.0f, 0.8f, 0.0f, 0.f);
		
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene_texture.bind_framebuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!is_menu && connected)
	{
		render_character(shaders[0], 
			game_camera, light, scene->models, player_count);
		render_type(shaders[0], game_camera, light, scene->models);

		shaders[6].use();
		skybox.render(shaders[6], game_camera);

		glDisable(GL_DEPTH_TEST);
		auto& s = shaders[5];
		if (debug_active)
		{
			s.use();
			s.uniform("projection", game_camera.projection);
			s.uniform("view", game_camera.view());
			line_debug(debug_positions);
			glEnable(GL_DEPTH_TEST);
		}
	}
	else if (!is_menu)
	{
		if(debug)
			render_character(shaders[0], 
				db_camera, light, scene->models, 4);
		render_type(shaders[0], db_camera, light, scene->models);

		shaders[6].use();
		skybox.render(shaders[6], db_camera);

		light_box.render(db_camera);

		if (debug_active)
		{
			glDisable(GL_DEPTH_TEST);
			auto& s = shaders[5];
			s.use();
			s.uniform("projection", db_camera.projection);
			s.uniform("view", db_camera.view());
			s.uniform("line_color", glm::vec3(1.0, 0.0, 0.0));
			line_debug(debug_positions);
			glEnable(GL_DEPTH_TEST);
		}
	}

	// Text
	shaders[2].use();
	if (is_chat_visible)
	{
		ui.render();
	}

	shaders[1].use();
	glm::mat4 projection = glm::ortho(0.0f, 1280.f, 0.0f, 720.f);
	shaders[1].uniform("projection", projection);
	shaders[1].uniform("text_color", glm::vec3(0.1f, 0.1f, 0.1f));

	auto offset = 0.0f;

	glDisable(GL_DEPTH_TEST);

	std::for_each(begin, end,
		[this, &offset, begin](const auto& s)
	{
		if (&s == begin || is_chat_visible)
			text.render_text(s.c_str(), 10.0f, (offset += 25.0f), 0.5f);
	});

	constexpr float size_y = static_cast<int>(720 / 12);

	for (auto i = 0u; i < buttons.size(); ++i)
		text.render_text(buttons[i], 10.0f, i * size_y, 1.0f);

	glEnable(GL_DEPTH_TEST);

	// Post Processing Effects
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shaders[3].use();
	shaders[3].uniform("scene_texture", 0);
	shaders[3].uniform("depth_texture", 1);
	shaders[3].uniform("screen_warning", 2);

	scene_texture.bind_texture(0);
	scene_texture.bind_texture(1);
	post_processing_effects.texture.bind(2);

	shaders[3].uniform("pulse", post_processing_effects.glow_value);
	post_processing_effects.render();
}

void Renderer::update(std::chrono::milliseconds delta,
	const glm::vec2& cursor,
	const std::array<glm::vec3, 4>& directions,
	const std::string& data,
	int num_players,
	int id,
	bool is_on,
	bool move_char)
{
	using namespace std::chrono_literals;
	time = data != log ? 0ms : time + delta;
	log = data;
	is_chat_visible = is_on || time < 3s;

	player_count = num_players;

	if (!is_on)
	{
		/*if (begin[0][button::glow] == button_state::pressed)
		{
			want_glow = !want_glow;
		}

		if (want_glow)
		{
			post_processing_effects.update(delta);
		}
		else
		{
			post_processing_effects.glow_value = 0;
		}*/

		db_camera.update(delta, directions[0], cursor);
	}

	if (scene->build_mode_active)
	{
		glm::vec2 build_pos[2];

		//game_camera.update(delta, &scene->v[scene->placing_object_id], &scene->v[scene->placing_object_id + 1]);
	}
	else
	{
		game_camera.update(delta, &scene->v[id], &scene->v[id + 1]);
	}

	ui.update();

	light.position += glm::vec3(sin(glfwGetTime()) / 10.f, 0.0, 0.0);
	light_box.set_position(light.position);
}

}