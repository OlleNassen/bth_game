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


	void update(const glm::vec2& start, const glm::vec2& end);
	void render(const Shader &shader, const Camera& cam)const;

private:
	glm::mat4 model {1.f};
	unsigned int vao;
	unsigned int vertex_buffer;

	float distance = 0.f;

	std::array<Texture, 10> laser_textures;
};

}


#endif
