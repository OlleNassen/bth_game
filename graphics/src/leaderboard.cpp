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

void Leaderboard::update(int array[])
{

}

void Leaderboard::render(const Shader& shader)const
{
	shader.use();
	shader.uniform("projection", projection);
	shader.uniform("text_color", glm::vec3(0.1f, 0.9f, 0.1f));

	std::stringstream test;
	float pos[4] = { 480, 400, 320, 240 };

	for (int i = 0; i < 4; i++)
	{
		test << "Player " << i << ": " << scoreboard[i] << "pt";
		text.render_text(test.str(), 1280 / 3.f, pos[i], 1.3f);

		test.str("");
	}
}

}
