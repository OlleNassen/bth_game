#ifndef LASER_HPP
#define LASER_HPP

#include <vector>
#include <array>
#include <glm/glm.hpp>
#include "shader.hpp"
#include "texture.hpp"
#include "primitives.hpp"

namespace graphics
{
class Laser
{
public:
	Laser();
	~Laser() = default;


	void update();
	void render(const Shader &shader, const Camera& cam)const;
	void disable();

private:
	unsigned int vao;
	unsigned int vertex_buffer;
	unsigned int laser_buffer;
	std::array<Texture, 10> laser_textures;
	Texture laser{"../resources/textures/laser/middle_background_texture.png", true};
	unsigned int active_texture;
};

}


#endif
