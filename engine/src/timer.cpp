#include "timer.hpp"

Timer::Timer(std::chrono::milliseconds time_left)
	: time_left{ time_left } 
{

}

bool Timer::is_up(std::chrono::milliseconds delta)
{ 
	return (time_left -= delta) <= std::chrono::milliseconds{ 0 }; 
}

std::string Timer::to_string() const
{
	const int minutes = static_cast<int>(time_left.count()) / 60000;
	const int seconds = (time_left.count() / 1000) % 60;
	if (minutes == 0 && seconds < 10)
	{
		return "0" + std::to_string(seconds);
	}
	else if (minutes == 0)
	{
		return std::to_string(seconds);
	}
	else if (seconds < 10)
	{
		return std::to_string(minutes) + ":" + "0" + std::to_string(seconds);
	}
	else
	{
		return std::to_string(minutes) + ":" + std::to_string(seconds);
	}
}