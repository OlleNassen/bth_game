#include "renderer.hpp"
#include <glm/gtc/matrix_transform.hpp>
Renderer::Renderer()
	:cam(90, 1280.f, 720.f, 0.1f, 100.f)
{
	models.reserve(sizeof(Model) * 2);
	models.emplace_back();
	models.emplace_back();
	texts.reserve(sizeof(Text) * 2);
	texts.emplace_back();
	shaders.reserve(sizeof(Shader) * 2);
	shaders.emplace_back("../resources/shaders/template.vs", "../resources/shaders/template.fs");
	shaders.emplace_back("../resources/shaders/text.vs", "../resources/shaders/text.fs");
}


void Renderer::render()const
{
	glClearColor(0.6f, 0.9f, 0.6f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	render_type(shaders[0], cam, models);

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
	cam.fps_update(delta, i);
	cam.mouse_movement(i.cursor);
}
