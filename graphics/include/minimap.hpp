#ifndef MINIMAP_HPP
#define MINIMAP_HPP
#include <vector>
#include "model.hpp"
#include "primitives.hpp"
#include "helpers.hpp"

namespace graphics
{

class Minimap
{
public:
	Minimap();
	~Minimap() = default;
	void update(const std::vector<Model>& models, int players);
	void render(const Shader& shader) const;
	//std::vector<float> positions;
	float positions[8];
private:
	//Shader minimap_shader;
	unsigned int vao_id;
	unsigned int vbo_id;

	//VertexArray vao_id;
	//Buffer vbo_id;
	unsigned int height = 100;
	unsigned int width = 10;
};

}
#endif // !MINIMAP_HPP

