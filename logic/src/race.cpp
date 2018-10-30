#include "race.hpp"

Race::Race(std::string* usernames)
{
	for (int i = 0; i < 4; ++i)
	{
		this->usernames[i] = usernames[i];
	}
}

bool Race::should_restart() const
{
	return false;
}

const std::string& Race::operator[](int index) const
{
	return usernames[player_ids[index]];
}

void Race::update(float* positions)
{
	for (int i = 0; i < 4; ++i)
	{
		if (positions[i] > goal)
			scores[i] += 1;
	}

	for (int i = 0; i < 4; ++i)
	{
		if (positions[i] > goal)
		{
			scores[player_ids[i]] += 1;
		}
			
	}

	for (int i = 0; i < 4; ++i)
	{
		int greatest = 0;
		for (int j = 0; j < 4; ++j)
		{
			if (positions[greatest] < positions[j])
				greatest = j;
		}
		positions[greatest] = 0.0f;
		player_ids[i] = greatest;
	}
}