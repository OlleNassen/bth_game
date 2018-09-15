#ifndef USER_INTERFACE_HPP
#define USER_IINTERFACE_HPP

#include <array>
#include <glm/glm.hpp>
#include "shader.hpp"


struct gui_element
{
	glm::vec2 position;
	glm::vec2 color;
};

class UserInterface
{
public:
	UserInterface();
	~UserInterface();

	void update();

	void render()const;
private:

	void rebind_buffers();

	unsigned int vao;
	unsigned int vertex_buffer;
	unsigned int gui_buffer;

	std::array<gui_element, 100> elements;
};

#endif
