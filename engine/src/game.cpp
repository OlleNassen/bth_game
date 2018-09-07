#include "game.hpp"
#include <iostream>

Game::Game()
	:window(glm::ivec2(1280, 720), "Untitled")
{
	renderer = new Renderer;
	shaders.emplace_back("../resources/shaders/template.vs", "../resources/shaders/template.fs");
}

Game::~Game()
{
}

void Game::run()
{
	while (window.is_open())
	{
		window.poll_events();
		update();
		render();
		window.swap_buffers();
	}
}

void Game::render()
{
	glClearColor(0.6f, 0.9f, 0.6f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (auto& shader: shaders)
	{
		shader.use();
		//Update uniforms here
		renderer->render();			
	}

}

void Game::update()
{
}
