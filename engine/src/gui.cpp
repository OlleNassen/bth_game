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

bool Chat::is_on() const
{
	return chat_on;
}

void Chat::update(std::chrono::milliseconds delta)
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

static void current_menu(int index, std::array<std::string, 12>& strings, const button_array& data)
{
	for (int i = 0; i < 12; ++i)
	{			
		if (data[i].text != "")
		{
			strings[i] = data[i].text;
			
			if (data[i].state == button_state::selected)
				strings[index] = "[" + strings[index] + "]";

			if (data[i].state == button_state::hover)
				strings[index] = " " + strings[index];
		}
		else
		{
			strings[i] = "";
		}
	}	
}

Menu::Menu() 
{
	buttons[0][10] = { "HOW TO PLAY", button_state::none };
	buttons[4][1] = { "BACK", button_state::none };

	buttons[0][8] = { "START", button_state::none };
	buttons[0][7] = { "OPTIONS", button_state::none };
	buttons[0][6] = { "EXIT", button_state::none };

	buttons[1][8] = { "HOST", button_state::none };
	buttons[1][7] = { "JOIN", button_state::none };
	buttons[1][6] = { "BACK", button_state::none };


	buttons[2][10] = { "FULLSCREEN", button_state::none };
	buttons[2][6] = { "BACK", button_state::none };
}

const std::array<std::string, 12>& Menu::button_strings() const
{
	return strings;
}

void Menu::open()
{
	if (current_buttons != &buttons[0])
	{
		current_buttons = &buttons[0];
		is_debug = false;
		is_on = true;
	}
	else
	{
		want_exit = true;
	}	
}

void Menu::update(std::chrono::milliseconds delta, const logic::input& i)
{
	auto index = logic::input::indices - i.index - 1;
	auto& button = (*current_buttons)[index];

	for (auto& arrays : buttons)
		for (auto& button : arrays)
			button.state = button_state::none;

	button.state = button_state::hover;
	fullscreen_pressed = false;
	
	if (i[logic::button::select] == logic::button_state::released)
	{
		if (current_buttons == &buttons[0] && index == 10)
		{
			current_buttons = &buttons[4];
		}
		else if (current_buttons == &buttons[0] && index == 8)
		{
			current_buttons = &buttons[1];
		}
		else if (current_buttons == &buttons[0] && index == 7)
		{
			current_buttons = &buttons[2];
		}
		else if (current_buttons == &buttons[0] && index == 6)
		{
			static constexpr auto true_dat = true;
			want_exit = true_dat;
		}
		else if (current_buttons == &buttons[1] && index == 8)
		{
			current_buttons = &buttons[3];
			is_on = false;
		}
		else if (current_buttons == &buttons[1] && index == 7)
		{
			current_buttons = &buttons[3];
			is_on = false;
			string_buffer = "";
			chat_on = true;
		}
		else if (current_buttons == &buttons[1] && index == 6)
		{
			current_buttons = &buttons[0];
		}	
		else if (current_buttons == &buttons[1] && index == 8)
		{
			current_buttons = &buttons[2];
			is_on = false;
		}
		else if (current_buttons == &buttons[2] && index == 10)
		{
			//fullscreen
			fullscreen_pressed = true;
		}
		else if (current_buttons == &buttons[2] && index == 6)
		{
			current_buttons = &buttons[0];
		}
		else if (current_buttons == &buttons[4] && index == 1)
		{
			current_buttons = &buttons[0];
		}
	}
	else if (i[logic::button::select] >= logic::button_state::pressed)
	{
		button.state = button_state::selected;		
	}	

	current_menu(index, strings, *current_buttons);
}

bool Menu::debug() const
{
	return is_debug;
}

bool Menu::on() const
{
	return is_on;
}

bool Menu::exit() const
{
	return want_exit;
}

bool Menu::get_fullscreen_pressed() const
{
	return fullscreen_pressed;
}

bool Menu::get_how_to_play() const
{
	return current_buttons == &buttons[4];
}

}