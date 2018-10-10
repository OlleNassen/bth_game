#ifndef SKYBOX_HPP
#define SKYBOX_HPP
#include "texture.hpp"
#include "shader.hpp"
#include "camera.hpp"

namespace graphics
{

class Skybox
{
public:
	Skybox();
	~Skybox() = default;

	void render(const Shader& shader, const Camera& camera)const;
private:
	unsigned int vao_id;
	unsigned int vbo_id;
	SkyboxTexture texture;
};

}


#endif