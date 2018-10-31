#ifndef LEADERBOARD_HPP
#define LEADERBOARD_HPP


#include <text.hpp>
#include <shader.hpp>

namespace graphics
{
class Leaderboard
{
public:
	Leaderboard(glm::mat4 projection);
	~Leaderboard();

	void render( Shader& shader);
	void update(int array[]);

private:
	int scoreboard[4];
	Text text;
	/*Shader leaderboard_text_shader{
		"../resources/shaders/text.vs",
		"../resources/shaders/text.fs" };*/
};

}

#endif