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

}


void Renderer::render()const
{
	glClearColor(0.6f, 0.9f, 0.6f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	render_type(shaders[0], camera, models);

	shaders[1].use();
	glm::mat4 projection = glm::ortho(0.0f, 1280.f, 0.0f, 720.f);
	shaders[1].uniform("projection", projection);
	shaders[1].uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));
	for (auto& text : texts)
	{
		text.render_text("HELLO, IS IT ME YOU'RE LOOKING FOR", 0, 0, 1);
	}
}

void Renderer::update(std::chrono::milliseconds delta, const input& i, int index)
{
	//camera.fps_update(delta, i);
	//camera.mouse_movement(i.cursor);

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

	models.front().move(offset);
	v[index] += offset;

	s_cam.update(delta, v, v+4);
}
