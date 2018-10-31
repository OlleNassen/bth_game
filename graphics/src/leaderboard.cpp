#include <leaderboard.hpp>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>

namespace graphics
{
Leaderboard::Leaderboard(const glm::mat4& projection)
{
	for (int i = 0; i < 4; i++)
		scoreboard[i] = i;

	this->projection = projection;
}

Leaderboard::~Leaderboard()
{
}

void Leaderboard::update(std::string&& scoreboard)
{
	this->scoreboard = scoreboard;
}

void Leaderboard::render(const Shader& shader,
	const Text& text)const
{
	shader.use();
	shader.uniform("projection", projection);
	shader.uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));

	text.render_text(scoreboard, 0, 0, 1.3f);
}

}
