#include "renderer.hpp"

Renderer::Renderer()
{
	models.emplace_back(new Model);
}

Renderer::~Renderer()
{
}

void Renderer::render()const
{
	//Render your models here
	models.back()->draw();
}
