#ifndef USER_INTERFACE_HPP
#define USER_IINTERFACE_HPP

#include <array>
#include <glm/glm.hpp>
#include "shader.hpp"
#include "helpers.hpp"


struct gui_element
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
private:

	void rebind_buffers();

	VertexArray vao;
	Buffer vertex_buffer;
	Buffer gui_buffer;

	std::array<gui_element, 100> elements;
};

#endif
