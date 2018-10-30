#include "race.hpp"

Race::Race(std::string* usernames)
{
	for (int i = 0; i < 4; i++)
	{
		name_id_score[i] = std::make_tuple(usernames[i], i, 0);
	}
}

bool Race::should_restart() const
{
	return false;
}

void Race::update(float* positions)
{
	for (int i = 0; i < 4; ++i)
	{
		if (positions[i] > goal)
		{
			std::get<2>(name_id_score[i]) += score;
			score -= 1;
		}
	}
}