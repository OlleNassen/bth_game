#ifndef INPUT_HPP
#define INPUT_HPP

#include <cstdint>
#include <map>
#include <glm/glm.hpp>

namespace logic
{

using uint16 = std::uint16_t;

enum class button	//DONT CHANGE!!!
{
	up,
	left,
	down,
	right,
	jump,
	debug,
	select,
	cancel,
	rotate,
	refresh,
	score,
	menu,
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
	int index = 0;
	glm::vec2 cursor;

	input() = default;

	explicit input(uint16 in)
	{
		for (int i = 0; i < static_cast<int>(button::quit) + 1; ++i)
		{
			if (in & (1 << i))
				data[i] = button_state::held;
		}
	}

	explicit operator uint16() const
	{
		uint16 result = 0;

		for (int i = 0; i < static_cast<int>(button::quit) + 1; ++i)
		{
			if (data[i] == button_state::held)
				result = (result | (1 << i));
		}

		return result;
	}

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

}

#endif


