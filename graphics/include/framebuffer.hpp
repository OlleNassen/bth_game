#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP
#include <GL/glew.h>
#include <iostream>
#include "texture.hpp"

namespace graphics
{

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

	void bind_framebuffer() const;
	void bind_texture() const;

	unsigned int fbo_texture;
	unsigned int fbo_depth;

private:
	unsigned int fbo;
};

}

#endif