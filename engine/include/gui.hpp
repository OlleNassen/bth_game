#ifndef GUI_HPP
#define GUI_HPP

#include <string>
#include <GLFW/glfw3.h>

namespace gui
{

const std::string& text_buffer();

void character_callback(GLFWwindow* window, unsigned int codepoint);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

}

#endif