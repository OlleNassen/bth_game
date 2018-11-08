#ifndef MINIMAP_HPP
#define MINIMAP_HPP
#include <vector>
#include <array>
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
	struct GuiElement
	{
		glm::vec2 position;
		glm::vec3 color;
		glm::vec2 scale;
	};

	void update(const std::vector<Model>& models, int players, float goal_height);
	void render(const Shader& shader) const;
	std::array<GuiElement, 10> map_elements;
	

private:
	
	VertexArray vao_id;
	Buffer vbo_id;
	glm::mat4 transform = glm::mat4(1.0f);
	unsigned int player_count = 0;
	unsigned int height = 100;
	unsigned int width = 10;
};

}
#endif // !MINIMAP_HPP

