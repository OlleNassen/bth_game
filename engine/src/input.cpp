#include "input.hpp"

input::input()
{

}

void input::update(GLFWwindow* glfw_window)
{
	for (auto&[key, value] : keybinds)
	{
		auto key_state = glfwGetKey(glfw_window, key);
		auto& button = buttons[static_cast<int>(value)];
		
		if (key_state == GLFW_PRESS)
		{
			button = button_state::pressed;
		}
		else
		{
			button = button_state::released;
		}
			
	}
}

button_state input::state(button b)
{
	return buttons[static_cast<int>(b)];
}


void input::assign_key(button name, int keybind)
{
	keybinds.insert(std::make_pair(keybind, name));
}

