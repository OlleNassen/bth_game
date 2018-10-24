#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

#include <vector>
#include <array>
#include <glm/glm.hpp>
#include "shader.hpp"
#include "helpers.hpp"
#include "texture.hpp"
#include "model.hpp"

namespace graphics
{

struct GuiElement
{
	glm::vec2 position;
	glm::vec2 color;
	glm::vec2 scale;
};

class PlayerArrows
{
public:
	PlayerArrows();
	~PlayerArrows() = default;
	void update(const std::vector<Model> &models, int players);
	void render();
	//void set_position(glm::vec2 position, int i)
private:
	Texture arrow_texture;
	std::array<glm::vec2, 4> player_positions;
};

class UserInterface
{
public:
	
	UserInterface();

	void update(const std::vector<Model> &models, int players);

	void render()const;
	PlayerArrows player_arrows;
private:

	void rebind_buffers();

	VertexArray vao;
	Buffer vertex_buffer;
	Buffer gui_buffer;

	std::array<GuiElement, 100> elements;
};

}

#endif
