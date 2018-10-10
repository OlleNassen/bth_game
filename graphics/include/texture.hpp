#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <string>

//Basic STB_image texture class

namespace graphics
{

class Texture
{
public:
	//Temp for HDR
	Texture();

	Texture(const std::string& path);
	~Texture();

	void bind(unsigned int index)const;
	void bind_hdr()
	{

	}
	void load_texture(const std::string& path);
	void load_hdr(const std::string& path);
private:
	unsigned int texture_id;

	int width;
	int height;
	int num_components;
};

}

#endif