#include "input.hpp"

void input_events::update(GLFWwindow* glfw_window)
{
	for (auto&[key, value] : keybinds)
	{
		auto key_state = glfwGetKey(glfw_window, key);
		auto& button = inputs[value];
		
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

input& input_events::state()
{
	return inputs;
}


void input_events::assign_key(button name, int keybind)
{
	keybinds.insert(std::make_pair(keybind, name));
}

