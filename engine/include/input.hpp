#ifndef INPUT_HPP
#define INPUT_HPP

#include <map>
#include <glm/glm.hpp>

enum class button
{
	
	up,
	left,
	down,
	right,
	jump,
	debug,
	select,
	cancel,
	glow,
	refresh,
	menu,	
	reset,
	quit
};

enum class button_state
{
	none,
	pressed,
	held,
	released
};

class input
{
public:	
	static constexpr int indices = 12;
	int num_players = 0;
	int index = 0;
	glm::vec2 cursor;

	const button_state& operator[](button b)const
	{
		return data[static_cast<int>(b)];
	}

	button_state& operator[](button b)
	{
		return data[static_cast<int>(b)];
	}

private:
	button_state data[static_cast<int>(button::quit) + 1];

};

#endif


