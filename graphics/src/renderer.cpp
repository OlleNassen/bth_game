#include "renderer.hpp"

Renderer::Renderer()
	:cam(90, 1280.f, 720.f, 0.f, 100.f)
{
	models.emplace_back(new Model);
	shaders.emplace_back("../resources/shaders/template.vs", "../resources/shaders/template.fs");
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
	}
}

void Renderer::update(std::chrono::milliseconds delta, const input& i)
{
	cam.update(delta, i);
	cam.mouse_movement(i.mouse_pos());
}
