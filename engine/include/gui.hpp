#ifndef GUI_HPP
#define GUI_HPP
#include <string>
#include "window.hpp"

class GraphicalUserInterface
{
public:
	GraphicalUserInterface();
	GraphicalUserInterface& operator = (const GraphicalUserInterface &) = delete;
	GraphicalUserInterface(const GraphicalUserInterface&) = delete;
	~GraphicalUserInterface();

	void update();
	std::string chat_update(const Window& window);

private:

	bool chat_mode{ false };
	std::string text;

};

#endif