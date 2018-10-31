#ifndef LEADERBOARD_HPP
#define LEADERBOARD_HPP
#include "text.hpp"
#include "shader.hpp"

namespace graphics
{

class Leaderboard
{
public:
	Leaderboard(const glm::mat4& projection);

	void render(const Shader& shader,
		const Text& text)const;
	void update(std::string&& scoreboard);

private:
	glm::mat4 projection;
	std::string scoreboard;

};

}

#endif