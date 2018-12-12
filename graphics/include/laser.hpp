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


	void update(
		const std::vector<glm::vec2>& starts, 
		const std::vector<glm::vec2>& ends);
	void render(const Shader &shader, const Camera& cam)const;

private:
	std::vector<glm::mat4> models;
	std::vector<float> distances;
	unsigned int vao;
	unsigned int vertex_buffer;

	std::array<Texture, 7> laser_textures;
};

}


#endif
