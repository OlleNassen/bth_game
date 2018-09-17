#ifndef POST_PROCESSING_EFFECTS_HPP
#define POST_PROCESSING_EFFECTS_HPP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <primitives.hpp>

class PostProcessingEffects
{
public:
	PostProcessingEffects();
	~PostProcessingEffects();

	void render()const;
	void update();

	float glow_value = 1;

private:

	unsigned int vao_id;
	unsigned int vbo_id;
};

#endif