#include "renderer.hpp"

Renderer::Renderer()
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
		for (auto& model : models)
		{
			model->render();
		}
	}
}