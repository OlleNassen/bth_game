#pragma once
#include "texture.hpp"
#include "shader.hpp"
#include "stb_image.h"
#include "primitives.hpp"
#include <GL/glew.h>
#include "glm/glm.hpp"
#include <chrono>

namespace graphics
{
	class FinishScreen
	{
	public:
		FinishScreen();
		~FinishScreen() = default;

		Texture finish_escaped;
		Texture finish_1_1;
		Texture finish_1_2;
		Texture finish_2_1;
		Texture finish_2_2;
		Texture finish_3_1;
		Texture finish_3_2;
		Texture finish_4;

		std::chrono::milliseconds timer{ 0 };

		void render(const Shader& finish_screen_shader, int finished) const;

	private:
		unsigned int vao_id;
		unsigned int vbo_id;
	};
}