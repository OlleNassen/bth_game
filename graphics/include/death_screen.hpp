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
	class DeathScreen
	{
	public:
		DeathScreen();
		~DeathScreen() = default;

		Texture death_1;
		Texture death_2;
		Texture death_3;
		Texture death_4;
		Texture death_5;
		Texture death_6;
		Texture death_7;

		std::chrono::milliseconds timer{ 0 };

		void render(const Shader& death_screen_shader) const;

	private:
		unsigned int vao_id;
		unsigned int vbo_id;
	};
}