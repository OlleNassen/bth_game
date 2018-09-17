#include "renderer.hpp"

#include <algorithm>
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

	shaders.reserve(sizeof(Shader) * 5);
	shaders.emplace_back("../resources/shaders/template.vs", "../resources/shaders/template.fs");
	shaders.emplace_back("../resources/shaders/text.vs", "../resources/shaders/text.fs");
	shaders.emplace_back("../resources/shaders/gui.vs", "../resources/shaders/gui.fs");
	//shaders.emplace_back("../resources/shaders/blinn_phong.vs", "../resources/shaders/blinn_phong.fs");
}


void Renderer::render(const std::string* begin, const std::string* end)const
{
	glClearColor(0.6f, 0.9f, 0.6f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	render_type(shaders[0], camera, models);

	shaders[2].use();
	if (is_chat_visible)
		ui.render();


	shaders[1].use();
	glm::mat4 projection = glm::ortho(0.0f, 1280.f, 0.0f, 720.f);
	shaders[1].uniform("projection", projection);
	shaders[1].uniform("text_color", glm::vec3(0.1f, 0.1f, 0.1f));

	auto offset = 0;
	
	std::for_each(begin, end,
		[this, &offset, begin](const auto& s)
		{
			if(&s == begin || is_chat_visible)
				text.render_text(s.c_str(), 10, (offset += 25), 0.5f);
		});
}

void Renderer::update(std::chrono::milliseconds delta, const input& i, int index, bool chat_on)
{
	using namespace std::chrono_literals;
	time += delta;
	//camera.fps_update(delta, i);
	//camera.mouse_movement(i.cursor);

	if (chat_on)
	{
		time = 0ms;
		is_chat_visible = true;
	}

	is_chat_visible = !(time > 5s);

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

	models[index].move(offset);
	v[index] += offset;

	s_cam.update(delta, v, v+4);
	ui.update();
}
