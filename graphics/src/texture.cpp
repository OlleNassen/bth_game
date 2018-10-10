#include "texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <gl/glew.h>

namespace graphics
{

Texture::Texture(const std::string& path)
{
	load_texture(path);
}

void Texture::bind(unsigned int index, GLenum type)const
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(type, texture_id);
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

SkyboxTexture::SkyboxTexture(const std::vector<std::string>& paths)

{
	load_texture(paths);
}

void SkyboxTexture::load_texture(const std::vector<std::string>& paths)
{
	stbi_set_flip_vertically_on_load(true);

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

	int i = 0;
	for(const auto& face: paths)
	{
		unsigned char *data = stbi_load(face.c_str(), &width, &height, &num_components, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i++, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << face << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

}
