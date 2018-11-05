#ifndef POST_PROCESSING_EFFECTS_HPP
#define POST_PROCESSING_EFFECTS_HPP
#include <GL/glew.h>
#include <iostream>
#include <primitives.hpp>
#include <chrono>
#include <texture.hpp>

namespace graphics
{

class PostProcessingEffects
{
public:
	PostProcessingEffects();
	~PostProcessingEffects();

	void render()const;
	void update(std::chrono::milliseconds delta);

	void initialize_textures();

	float glow_value = 0;

	Texture texture;

	//Texture data
	int width = 0, height = 0, nrOfChannels = 0;
	unsigned char* data = nullptr;

	//Textures
	unsigned int screen_warning;

private:
	std::chrono::duration<float> duration{0};
	unsigned int vao_id;
	unsigned int vbo_id;
};

}

#endif