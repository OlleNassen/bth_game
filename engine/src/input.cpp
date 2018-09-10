#include "input.hpp"

glm::vec2 input_events::mouse_pos() const
{
	return mouse_position;
}

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

	double x = 0.0;
	double y = 0.0;
	glfwGetCursorPos(glfw_window, &x, &y);
	mouse_position = { x, y };
}

const input& input_events::state() const
{
	return inputs;
}


void input_events::assign_key(button name, int keybind)
{
	keybinds.insert(std::make_pair(keybind, name));
}

