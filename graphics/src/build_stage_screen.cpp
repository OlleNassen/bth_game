#include "build_stage_screen.hpp"

namespace graphics
{
	BuildStageScreen::BuildStageScreen()
		:build_1("../resources/textures/build_stage_screen/build_stage.png", true)

	{
		this->transparency = 0.8f;
		glGenVertexArrays(1, &vao_id);
		glBindVertexArray(vao_id);
		glGenBuffers(1, &vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(primitives::quad_uv), &primitives::quad_uv, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
		glEnableVertexAttribArray(0);
	}

	void graphics::BuildStageScreen::render(const Shader& build_stage_screen_shader) const
	{
		using namespace std::chrono_literals;
		build_stage_screen_shader.use();
		build_stage_screen_shader.uniform("overlay_texture", 0);
		this->build_1.bind(0);
		build_stage_screen_shader.uniform("transparency", this->transparency);
		

		glBindVertexArray(vao_id);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}