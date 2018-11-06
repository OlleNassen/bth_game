#include <finish_screen.hpp>

namespace graphics
{
	FinishScreen::FinishScreen()
		:finish_escaped("../resources/textures/finish_screen/finish_escaped.png")
		,finish_1_1("../resources/textures/finish_screen/finish_1.1.png")
		,finish_1_2("../resources/textures/finish_screen/finish_1.2.png")
		,finish_2_1("../resources/textures/finish_screen/finish_2.1.png")
		,finish_2_2("../resources/textures/finish_screen/finish_2.2.png")
		,finish_3_1("../resources/textures/finish_screen/finish_3.1.png")
		,finish_3_2("../resources/textures/finish_screen/finish_3.2.png")
		,finish_4("../resources/textures/finish_screen/finish_4.png")

	{
		glGenVertexArrays(1, &vao_id);
		glBindVertexArray(vao_id);
		glGenBuffers(1, &vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(primitives::quad_uv), &primitives::quad_uv, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
		glEnableVertexAttribArray(0);
	}

	void graphics::FinishScreen::render(const Shader& finish_screen_shader, int finished) const
	{
		using namespace std::chrono_literals;
		finish_screen_shader.use();
		finish_screen_shader.uniform("finish_texture", 0);

		if (finished == 0)
		{
			if (timer <= 1500ms)
			{
				this->finish_escaped.bind(0);
			}
			else if (timer > 1500ms && timer <= 3000ms)
			{
				this->finish_1_1.bind(0);
			}
			else if (timer > 3000ms)
			{
				this->finish_1_2.bind(0);
			}
		}
		else if (finished == 1)
		{
			if (timer <= 1500ms)
			{
				this->finish_escaped.bind(0);
			}
			else if (timer > 1500ms && timer <= 3000ms)
			{
				this->finish_2_1.bind(0);
			}
			else if (timer > 3000ms)
			{
				this->finish_2_2.bind(0);
			}
		}
		else if (finished == 2)
		{
			if (timer <= 1500ms)
			{
				this->finish_escaped.bind(0);
			}
			else if (timer > 1500ms && timer <= 3000ms)
			{
				this->finish_3_1.bind(0);
			}
			else if (timer > 3000ms)
			{
				this->finish_3_2.bind(0);
			}
		}
		else if (finished == 3)
		{
			if (timer <= 1500ms)
			{
				this->finish_escaped.bind(0);
			}
			else if (timer > 1500ms)
			{
				this->finish_4.bind(0);
			}
		}

		glBindVertexArray(vao_id);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}