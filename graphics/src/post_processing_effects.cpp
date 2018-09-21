#include "post_processing_effects.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

PostProcessingEffects::PostProcessingEffects()
{
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(primitives::quad_uv), &primitives::quad_uv, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
	glEnableVertexAttribArray(0);

	initialize_textures();
}

PostProcessingEffects::~PostProcessingEffects()
{
}

void PostProcessingEffects::render() const
{
	glBindVertexArray(vao_id);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void PostProcessingEffects::update()
{
		glow_value = cos(2 * glfwGetTime());
}

void PostProcessingEffects::initialize_textures()
{
	data = stbi_load("../resources/textures/screen_warning.png", &width, &height, &nrOfChannels, 0);
	
	glGenTextures(1, &screen_warning);
	glBindTexture(GL_TEXTURE_2D, screen_warning);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load Texture from path" << std::endl;
	}
	stbi_image_free(data);
}
