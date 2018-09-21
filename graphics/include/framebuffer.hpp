#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP
#include <GL/glew.h>
#include <iostream>

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

	void bind_framebuffer() const;
	void bind_texture() const;

	unsigned int fbo_texture;

private:
	unsigned int fbo;
};

#endif