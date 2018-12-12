#ifndef LASER_HPP
#define LASER_HPP

#include <vector>
#include <array>
#include <glm/glm.hpp>
#include "shader.hpp"
#include "helpers.hpp"
#include "texture.hpp"
#include "model.hpp"

namespace graphics
{
class Laser
{
private:
	void rebind_buffers();

	VertexArray vao;
	Buffer vertex_buffer;
	Buffer laser_buffer;
	std::array<Texture, 10> laser_textures;
	unsigned int active_texture;
public:
	Laser();
	~Laser();


	void update();
	void render(const Shader &shader)const;
	void disable();

};

}


#endif
