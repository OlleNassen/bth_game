#include "input.hpp"

input::input()
{

}

void input::bind(button b, callback c)
{
	callbacks[static_cast<int>(b)] = c;
}

void input::unbind(button b)
{
	callbacks[static_cast<int>(b)] = callback{};
}
