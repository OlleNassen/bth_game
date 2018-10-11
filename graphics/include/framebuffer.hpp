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
	void bind_texture(int index) const;
private:
	unsigned int fbo_texture;
	unsigned int depth_texture;
	unsigned int fbo;

};

}

#endif