#ifndef RACE_HPP
#define RACE_HPP

#include <string>

class Race
{
public:
	Race() = default;
	Race(std::string* usernames);

	bool should_restart() const;

	const std::string& operator[](int index) const;
	void update(float* positions);

private:
	float goal = 10.0;
	std::string usernames[4] = { "P1", "P2", "P3", "P4" };
	int player_ids[4] = { 0, 1, 2, 3 };
	int scores[4] = {};
};


#endif
