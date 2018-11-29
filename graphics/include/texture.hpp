#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <string>
#include <vector>
#include <gl/glew.h>

//Basic STB_image texture class

namespace graphics
{

class Texture
{
public:
	Texture() = default;
	Texture(const std::string& path, bool alpha);
	~Texture() = default;

	void bind(unsigned int index, 
		GLenum type = GL_TEXTURE_2D)const;
	
	void load_texture(const std::string& path, bool alpha);

protected:
	unsigned int texture_id;
	int width;
	int height;
	int num_components;
};

class SkyboxTexture : public Texture
{
public:
	SkyboxTexture(const std::vector<std::string>& paths);

	//void bind(unsigned int index)const override;
	void load_texture(const std::vector<std::string>& paths);
};


}

#endif