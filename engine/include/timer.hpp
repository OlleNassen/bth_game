#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

class Timer
{
public:
	Timer(std::chrono::milliseconds time_left) : time_left{time_left} {}
	bool is_up(std::chrono::milliseconds delta) { return (time_left -= delta) < std::chrono::milliseconds{0}; }

private:
	std::chrono::milliseconds time_left;
};

#endif
