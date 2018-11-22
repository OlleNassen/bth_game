#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP
#include <GL/glew.h>
#include <iostream>
#include "texture.hpp"
#include "shader.hpp"
#include "skybox.hpp"

namespace graphics
{

class Framebuffer
{
public:
	Framebuffer();
	Framebuffer(const Shader& shader, const Skybox& skybox); //Irradiance buffer
	Framebuffer(const Shader& shader, const Skybox& skybox, bool temp); //Pre_Filter buffer
	Framebuffer(const Shader& shader, const Skybox& skybox, float temp); //BRDF Something buffer

	~Framebuffer();
	unsigned int quadVAO = 0;
	unsigned int quadVBO = 0;

	void create_quad()
	{
		if (quadVAO == 0 && quadVBO == 0)
		{
			float quadVertices[] =
			{
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
	}

	void render_quad()
	{
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	void bind_framebuffer() const;
	void bind_texture(int index) const;
private:
	unsigned int fbo;

	unsigned int fbo_texture;
	unsigned int depth_texture;
	unsigned int irradiance_map;
	unsigned int prefilter_map;
	unsigned int brdf_lut;
};

}

#endif