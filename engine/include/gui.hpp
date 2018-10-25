#ifndef GUI_HPP
#define GUI_HPP

#include <chrono>
#include <array>
#include <string>
#include <GLFW/glfw3.h>
#include <input.hpp>

namespace gui
{

void character_callback(GLFWwindow* window, unsigned int codepoint);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

class Chat
{
public:	
	bool is_on() const;
	
	void update(std::chrono::milliseconds delta);
	
	const std::string& operator[](int i) const { return chat_log[i]; }
	const std::string* begin() const { return &chat_log[0]; }
	const std::string* end() const { return &chat_log[10]; }

	std::string* begin() { return &chat_log[0]; }
	std::string* end() { return &chat_log[10]; }

private:	
	std::string chat_log[10];	
	std::chrono::milliseconds time{0};
	bool has_underscore = false;
};

enum class button_state
{
	none,
	hover,
	selected
};

struct Button
{
	std::string text = "";
	button_state state = button_state::none;
};

using button_array = std::array<gui::Button, logic::input::indices>;

class Menu
{
public:	
	Menu();
	void update(std::chrono::milliseconds delta, const logic::input& i);

	const std::array<std::string, 12>& button_strings() const;

	bool debug() const;

	bool on() const;

	bool exit() const;

private:
	bool is_debug = false;
	bool is_on = true;
	bool want_exit = false;
	button_array buttons[3];
	button_array* current_buttons{&buttons[0]};

	std::array<std::string, 12> strings;
};

class Options
{
public:
	Options();
	void update(std::chrono::milliseconds delta, const logic::input& i);

	const std::array<std::string, 12>& button_strings() const;

	bool debug() const;

	bool on() const;

	bool exit() const;

private:
	bool is_debug = false;
	bool is_on = false;
	bool want_exit = false;
	button_array buttons[3];
	button_array* current_buttons{ &buttons[0] };

	std::array<std::string, 12> strings;
};

}

#endif