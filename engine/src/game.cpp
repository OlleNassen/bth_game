#include "game.hpp"
#include <iostream>

Game::Game()
	:window(glm::ivec2(1280, 720), "Untitled")
{
	renderer = new Renderer;
	shaders.emplace_back("../resources/shaders/template.vs", "../resources/shaders/template.fs");

	player_input.assign_key(button::up, GLFW_KEY_W);
	player_input.assign_key(button::left, GLFW_KEY_A);
	player_input.assign_key(button::down, GLFW_KEY_S);
	player_input.assign_key(button::right, GLFW_KEY_D);
	window.bind(player_input);
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
	using std::cout;
	constexpr char nl = '\n';
	
	if (player_input.state(button::up) == button_state::pressed)
	{
		cout << "up" << nl;
	}
	if (player_input.state(button::left) == button_state::pressed)
	{
		cout << "left" << nl;
	}
	if (player_input.state(button::down) == button_state::pressed)
	{
		cout << "down" << nl;
	}
	if (player_input.state(button::right) == button_state::pressed)
	{
		cout << "right" << nl;
	}

}
