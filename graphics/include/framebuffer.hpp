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

	void bind_framebuffer() const;
	void bind_texture(int index) const;
private:
	unsigned int fbo;

	unsigned int fbo_texture;
	unsigned int depth_texture;
	unsigned int irradiance_map;
	unsigned int prefilter_map;
	unsigned int brdfLUTTexture;
};

}

#endif