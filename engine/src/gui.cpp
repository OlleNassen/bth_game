#include "gui.hpp"

namespace gui
{

std::string string_buffer;

void clear_buffer()
{
	string_buffer.clear();
}

const std::string& text_buffer()
{
	return string_buffer;
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
	string_buffer += codepoint;	
}

}