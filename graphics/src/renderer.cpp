#include "renderer.hpp"
#include <glm/gtc/matrix_transform.hpp>
Renderer::Renderer()
	:cam(90, 1280.f, 720.f, 0.f, 100.f)
{
	models.emplace_back(new Model);
	shaders.reserve(sizeof(Shader) * 2);
	shaders.emplace_back("../resources/shaders/template.vs", "../resources/shaders/template.fs");
	shaders.emplace_back("../resources/shaders/text.vs", "../resources/shaders/text.fs");
}

Renderer::~Renderer()
{
	while (models.size() > 0)
	{
		delete models.back();
		models.pop_back();
	}
}

void Renderer::render()const
{
	glClearColor(0.6f, 0.9f, 0.6f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);
	//Render your models here
	/*
	for (auto& shader : shaders)
	{
		shader.use();
		//Update uniforms here
		glm::mat4 model{ 1.f };
		shader.uniform("model", model);
		shader.uniform("view", cam.view_matrix());
		shader.uniform("projection", cam.projection_matrix());
		for (auto& model : models)
		{
			model->render();
		}
		test_text.render_text("HELLO, IS IT ME YOU'RE LOOKING FOR", 0, 0, 1);
	}
	*/
	shaders[0].use();
	//Update uniforms here
	glm::mat4 model{ 1.f };
	shaders[0].uniform("model", model);
	shaders[0].uniform("view", cam.view_matrix());
	shaders[0].uniform("projection", cam.projection_matrix());
	models.back()->render();

	shaders[1].use();
	glm::mat4 projection = glm::ortho(0.0f, 1280.f, 0.0f, 720.f);
	shaders[1].uniform("projection", projection);
	shaders[1].uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));
	test_text.render_text("HELLO, IS IT ME YOU'RE LOOKING FOR", 0, 0, 1);
}

void Renderer::update(std::chrono::milliseconds delta, const input& i)
{
	cam.fps_update(delta, i);
	cam.mouse_movement(i.cursor);
}
