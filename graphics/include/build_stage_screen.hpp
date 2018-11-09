#ifndef BUILD_STAGE_SCREEN_HPP
#define BUILD_STAGE_SCREEN_HPP
#include "texture.hpp"
#include "shader.hpp"
#include "stb_image.h"
#include "primitives.hpp"
#include <GL/glew.h>
#include "glm/glm.hpp"
#include <chrono>

namespace graphics
{
	class BuildStageScreen
	{
	public:
		BuildStageScreen();
		~BuildStageScreen() = default;

		Texture build_1;
		float transparency;

		std::chrono::milliseconds timer{ 0 };

		void render(const Shader& build_stage_screen_shader) const;

	private:
		unsigned int vao_id;
		unsigned int vbo_id;
	};
}

#endif