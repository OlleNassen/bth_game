#include "texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <gl\glew.h>

Texture::Texture(const std::string& path)
{
	load_texture(path);
}

Texture::~Texture()
{

}

void Texture::bind(unsigned int index)const
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, texture_id);
}

void Texture::load_texture(const std::string & path)
{
	stbi_set_flip_vertically_on_load(true);

	unsigned char* imageData = stbi_load(path.c_str(), &width, &height, &num_components, 4);

	/* Error handling. */
	if (imageData == nullptr)
	{
		//std::cout << ("Failed to load texture: " + path + "\n");
	}

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,
		height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	stbi_image_free(imageData);
}
