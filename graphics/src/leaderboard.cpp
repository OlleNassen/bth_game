#include <leaderboard.hpp>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>

namespace graphics
{
Leaderboard::Leaderboard(glm::mat4 projection)
{
	for (int i = 0; i < 4; i++)
		scoreboard[i] = i;
}

Leaderboard::~Leaderboard()
{
}

void Leaderboard::update(int array[])
{

}

void Leaderboard::render( Shader& shader)
{
	shader.use();
	glm::mat4 projection = glm::ortho(0.0f, 1280.f, 0.0f, 720.f);
	shader.uniform("projection", projection);
	/*shader.uniform("text_color", glm::vec3(0.1f, 0.1f, 0.1f));*/

	std::stringstream test;
	float pos[4] = { 480, 400, 320, 240 };
	shader.uniform("text_color", glm::vec3(0.1f, 0.9f, 0.1f));

	for (int i = 0; i < 4; i++)
	{
		test << "Player " << i << ": " << scoreboard[i] << "pt";
		text.render_text(test.str(), 1280 / 3.f, pos[i], 1.3f);

		test.str("");
	}
}

}
