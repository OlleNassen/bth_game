#include "minimap.hpp"


graphics::Minimap::Minimap()
{
}

void graphics::Minimap::update(const std::vector<Model>& models, int players)
{
	for (int i = 0; i < players; i++)
	{
		this->positions[i] = models[i].get_y_position();
		std::cout << positions[i] << std::endl;

	}
}

void graphics::Minimap::render(const Shader& shader)
{
}

graphics::Minimap::~Minimap()
{
}
