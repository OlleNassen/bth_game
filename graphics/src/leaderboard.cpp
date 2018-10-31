#include "leaderboard.hpp"

namespace graphics
{
Leaderboard::Leaderboard(const glm::mat4& projection)
	: projection{projection}
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
