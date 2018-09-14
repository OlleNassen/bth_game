#include "gui.hpp"

namespace gui
{

std::string string_buffer;

const std::string& text_buffer()
{
	return string_buffer;
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
	string_buffer += codepoint;	
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ENTER: string_buffer.clear(); break;
		case GLFW_KEY_BACKSPACE: string_buffer.pop_back(); break;
		default: break;
		}
	}
	
}

}