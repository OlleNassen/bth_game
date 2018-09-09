#ifndef INPUT_HPP
#define INPUT_HPP

#include <functional>

enum class button
{
	up,
	left,
	down,
	right,
	count
};


struct callback
{
	std::function<void()> pressed = [] {};
	std::function<void()> released = [] {};
};


class input
{
public:
	input();

	void bind(button b, callback c);
	void unbind(button b);

private:
	callback callbacks[static_cast<int>(button::count)];
};

#endif


