#include "renderer.hpp"
#include <glm/gtc/matrix_transform.hpp>
Renderer::Renderer()
	: camera(90, 1280.f, 720.f, 0.1f, 100.f)
	, s_cam(90, 1280.f, 720.f, 0.1f, 100.f)
{
	using glm::vec3;
	glm::mat4 model{ 1.0f };
	
	v[0] = { 10, 10 };
	v[1] = { -5, -5 };
	v[2] = { 14, 2 };
	v[3] = { -4, -20 };
	models.reserve(sizeof(Model) * 4);
	models.emplace_back(glm::translate(model, vec3{ v[0], 0 }));
	models.emplace_back(glm::translate(model, vec3{ v[1], 0 }));
	models.emplace_back(glm::translate(model, vec3{ v[2], 0 }));
	models.emplace_back(glm::translate(model, vec3{ v[3], 0 }));

	texts.reserve(sizeof(Text) * 2);
	texts.emplace_back();
	shaders.reserve(sizeof(Shader) * 2);
	shaders.emplace_back("../resources/shaders/template.vs", "../resources/shaders/template.fs");
	shaders.emplace_back("../resources/shaders/text.vs", "../resources/shaders/text.fs");
	shaders.emplace_back("../resources/shaders/blinn_phong.vs", "../resources/shaders/blinn_phong.fs");
	shaders.emplace_back("../resources/shaders/post_processing_effects.vs", "../resources/shaders/post_processing_effects.fs"); //3

}


void Renderer::render()const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.6f, 0.6f, 0.6f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	scene_texture.bind_framebuffer();
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_TRUE);

	render_type(shaders[0], camera, models);

	// Post Processing Effects
	glDisable(GL_DEPTH_TEST);
	shaders[3].use();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	shaders[3].uniform("scene_texture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, scene_texture.fbo_texture);
	shaders[3].uniform("screen_warning", 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, post_processing_effects.screen_warning);

	shaders[3].uniform("pulse", post_processing_effects.glow_value);
	post_processing_effects.render();

	// Text
	shaders[1].use();
	glm::mat4 projection = glm::ortho(0.0f, 1280.f, 0.0f, 720.f);
	shaders[1].uniform("projection", projection);
	shaders[1].uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));
	for (auto& text : texts)
	{
		text.render_text("HELLO, IS IT ME YOU'RE LOOKING FOR", 0, 0, 1);
	}

}

void Renderer::update(std::chrono::milliseconds delta, const input& i)
{
	camera.fps_update(delta, i);
	camera.mouse_movement(i.cursor);

	using glm::vec2;
	float speed{ 10.f };
	vec2 offset{ 0.0f, 0.0f };
	float dt = delta.count() / 1000.0f;

	if (i[button::up] >= button_state::pressed)
	{
		offset += vec2{ 0, speed } * dt;
	}
	if (i[button::left] >= button_state::pressed)
	{
		offset += vec2{ -speed, 0 } * dt;
	}
	if (i[button::down] >= button_state::pressed)
	{
		offset += vec2{ 0, -speed } * dt;
	}
	if (i[button::right] >= button_state::pressed)
	{
		offset += vec2{ speed, 0 } * dt;
	}
	if (i[button::glow] == button_state::pressed)
	{
		want_glow = !want_glow;
	}
	
	if (want_glow)
	{
		post_processing_effects.update();
	}
	else
	{
		post_processing_effects.glow_value = 0;
	}

	models.front().move(offset);
	v[0] += offset;

	s_cam.update(delta, v, v+4);
}
