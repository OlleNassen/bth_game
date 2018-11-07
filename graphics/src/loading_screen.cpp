#include <loading_screen.hpp>

namespace graphics
{
LoadingScreen::LoadingScreen()
	:waiting_1("../resources/textures/loading_screen/waiting_1.png")
	,waiting_2("../resources/textures/loading_screen/waiting_2.png")
	,waiting_3("../resources/textures/loading_screen/waiting_3.png")
	,waiting_4("../resources/textures/loading_screen/waiting_4.png")

{
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(primitives::quad_uv), &primitives::quad_uv, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
	glEnableVertexAttribArray(0);
}

void graphics::LoadingScreen::render(const Shader& loading_screen_shader) const
{
	using namespace std::chrono_literals;
	loading_screen_shader.use();
	if (timer <= 1000ms)
	{
		loading_screen_shader.uniform("loading_texture", 0);
		this->waiting_1.bind(0);
	}
	else if (timer >= 1000ms && timer <= 2000ms)
	{
		loading_screen_shader.uniform("loading_texture", 0);
		this->waiting_2.bind(0);
	}
	else if (timer >= 2000ms && timer <= 3000ms)
	{
		loading_screen_shader.uniform("loading_texture", 0);
		this->waiting_3.bind(0);
	}
	else if (timer >= 3000ms)
	{
		loading_screen_shader.uniform("loading_texture", 0);
		this->waiting_4.bind(0);
	}

	glBindVertexArray(vao_id);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
}