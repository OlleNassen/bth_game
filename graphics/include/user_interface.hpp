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
	float angle;
};

class PlayerArrows
{
public:
	PlayerArrows();
	~PlayerArrows() = default;
	void update(const std::vector<Model> &models, int players, std::array<GuiElement, 100> &elements);
	void render() const;
	//void set_position(glm::vec2 position, int i)
	glm::mat4 arrow_matrix;
private:
	std::array<glm::vec2, 4> player_positions;
	std::array<glm::vec2, 4> player_vector;
	std::array<bool, 4> visible;
};

class UserInterface
{
public:
	
	UserInterface();

	void update(const std::vector<Model> &models, int players);
	/*void render_arrows();
	void render_chat();*/
	void render(const Shader &shader)const;
	void disable_chat();
	void enable_chat();
	PlayerArrows player_arrows;
private:

	void rebind_buffers();

	VertexArray vao;
	Buffer vertex_buffer;
	Buffer gui_buffer;
	std::array<Texture, 10> arrow_textures;
	std::array<GuiElement, 100> elements;
};

}

#endif
