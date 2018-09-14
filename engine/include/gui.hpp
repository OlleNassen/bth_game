#ifndef GUI_HPP
#define GUI_HPP

#include <string>
#include <GLFW/glfw3.h>

namespace gui
{

void clear_buffer();
const std::string& text_buffer();

void character_callback(GLFWwindow* window, unsigned int codepoint);

}

#endif