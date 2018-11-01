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
class LoadingScreen
{
public:
	LoadingScreen();
	~LoadingScreen() = default;

	Texture waiting_1;
	Texture waiting_2;
	Texture waiting_3;
	Texture waiting_4;

	std::chrono::milliseconds timer{0};

	float g_vertex_buffer_data[12] = { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f,  0.5f, 0.0f, 0.5f,  0.5f, 0.0f };

	void render(const Shader& loading_screen_shader) const;

private:
	unsigned int vao_id;
	unsigned int vbo_id;
};
}