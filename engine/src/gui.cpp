#include "gui.hpp"
#include <iostream>

namespace gui
{

using namespace std::chrono_literals;

std::string string_buffer;
bool chat_on = false;

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
	if (action != GLFW_RELEASE && key == GLFW_KEY_BACKSPACE && !string_buffer.empty())
	{
		string_buffer.pop_back();
	}
	else if (action == GLFW_PRESS && key == GLFW_KEY_ENTER)
	{
		chat_on = !chat_on;
		string_buffer.clear();
	}	
}

bool chat::is_on() const
{
	return chat_on;
}

void chat::update(std::chrono::milliseconds delta)
{
	constexpr auto underscore = '_';
	
	if (!chat_on && chat_log[0].size() > 1 && string_buffer.empty())
	{
		if (has_underscore)
		{
			chat_log[0].pop_back();
			std::rotate(begin(), end() - 1, end());
			chat_log[0] = "";
		}
		else
		{
			std::rotate(begin(), end() - 1, end());
			chat_log[0] = "";
		}
	}

	if (has_underscore)
		chat_log[0] = string_buffer + underscore;
	else
		chat_log[0] = string_buffer;

	time += delta;

	if (time > 500ms)
	{
		has_underscore = !has_underscore;
		time = 0ms;
	}

	if (!chat_on)
	{
		chat_log[0].clear();
	}
}

Menu::Menu() 
{
	buttons[0][8] = { "Start", button_state::none };
	buttons[0][7] = { "Options", button_state::none };
	buttons[0][6] = { "Exit", button_state::none };

	buttons[1][8] = { "Host", button_state::none };
	buttons[1][7] = { "Join", button_state::none };
	buttons[1][6] = { "Back", button_state::none };
}

const gui::button_array& Menu::button_data() const
{
	return *current_buttons;
}

void Menu::update(std::chrono::milliseconds delta, const input& i)
{
	auto index = input::indices - i.index - 1;
	auto& button = (*current_buttons)[index];
	
	for (auto& arrays : buttons)
		for (auto& button : arrays)
			button.state = button_state::none;

	button.state = button_state::hover;
	
	if (i[::button::select] == ::button_state::released)
	{
		if (current_buttons == &buttons[0] && index == 8)
		{
			current_buttons = &buttons[1];
		}
		if (current_buttons == &buttons[1] && index == 6)
		{
			current_buttons = &buttons[0];
		}
	}
	else if (i[::button::select] >= ::button_state::pressed)
	{
		button.state = button_state::selected;		
	}	
}

}