#ifndef GUI_HPP
#define GUI_HPP

#include <chrono>
#include <string>
#include <GLFW/glfw3.h>

namespace gui
{

void character_callback(GLFWwindow* window, unsigned int codepoint);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

class chat
{
public:	
	void update(std::chrono::milliseconds delta);
	
	const std::string& operator[](int i) const { return chat_log[i]; }
	const std::string* begin() const { return &chat_log[0]; }
	const std::string* end() const { return &chat_log[10]; }

private:	
	std::string chat_log[10];	
	std::chrono::milliseconds time{0};
	bool has_underscore = false;
};

}

#endif