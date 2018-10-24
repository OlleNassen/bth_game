#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

#include <array>
#include <glm/glm.hpp>
#include "shader.hpp"
#include "helpers.hpp"

namespace graphics
{

struct GuiElement
{
	glm::vec2 position;
	glm::vec2 color;
	glm::vec2 scale;
};

class UserInterface
{
public:
	UserInterface();

	void update();

	void render()const;
protected:

	void rebind_buffers();

	VertexArray vao;
	Buffer vertex_buffer;
	Buffer gui_buffer;

	std::array<GuiElement, 100> elements;
};

}

#endif
