#include "renderer.hpp"

Renderer::Renderer()
{
	models.emplace_back(new Model);
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
	//Render your models here
	models.back()->draw();
}
