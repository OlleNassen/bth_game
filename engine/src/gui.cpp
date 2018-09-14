#include "gui.hpp"
#include <iostream>
GraphicalUserInterface::GraphicalUserInterface()
{
}

GraphicalUserInterface::~GraphicalUserInterface()
{
}

void GraphicalUserInterface::update()
{



}

std::string GraphicalUserInterface::chat_update(const Window& window)
{
	if (!chat_mode)
	{
		if (!text.empty())
		{
			std::string temp = text;
			text.clear();
			return temp;
		}
		if (glfwGetKey(window.glfw_window, GLFW_KEY_ENTER) == GLFW_PRESS)
			chat_mode = true;
	}
	else if (chat_mode)
	{
		if (glfwGetKey(window.glfw_window, GLFW_KEY_A) == GLFW_PRESS)
		{
			text.push_back('A');
		}
		else if (glfwGetKey(window.glfw_window, GLFW_KEY_B) == GLFW_PRESS)
		{
			text.push_back('B');
		}
		else if (glfwGetKey(window.glfw_window, GLFW_KEY_BACKSPACE) == GLFW_PRESS)
		{
			chat_mode = false;
		}
	}

	return "";
}
