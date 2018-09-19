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

	void initialize_textures();

	float glow_value = 1;

	//Texture data
	int width = 0, height = 0, nrOfChannels = 0;
	unsigned char* data = nullptr;

	//Textures
	unsigned int screen_warning;

private:

	unsigned int vao_id;
	unsigned int vbo_id;
};

#endif