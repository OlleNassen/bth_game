#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include <string>

class Timer
{
public:
	Timer(std::chrono::milliseconds time_left);
	bool is_up(std::chrono::milliseconds delta);

	std::string to_string() const;

private:
	std::chrono::milliseconds time_left;
};

#endif
