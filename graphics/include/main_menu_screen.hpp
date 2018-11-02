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
	class MainMenuScreen
	{
	public:
		MainMenuScreen();
		~MainMenuScreen() = default;

		Texture main_1;
		Texture main_2;

		std::chrono::milliseconds timer{ 0 };

		void render(const Shader& main_menu_shader) const;

	private:
		unsigned int vao_id;
		unsigned int vbo_id;
	};
}