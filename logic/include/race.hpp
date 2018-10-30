#ifndef RACE_HPP
#define RACE_HPP
#include <string>
#include <tuple>
#include <array>

class Race
{
public:
	Race() = default;
	Race(std::string* usernames);

	bool should_restart() const;

	void update(float* positions);

private:
	float goal = 10.0;
	float score = 5; 
	std::string usernames[4] = { "P1", "P2", "P3", "P4" };
	std::array<std::tuple<std::string, int, int>, 4> name_id_score;
};


#endif
