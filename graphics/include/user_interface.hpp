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
	glm::vec3 color;
	glm::vec2 scale;
	float angle;
};

class PlayerArrows
{
public:
	PlayerArrows();
	~PlayerArrows() = default;
	void update(const std::vector<Model> &models, int players, std::array<GuiElement, 100> &elements, glm::vec3 &camera_pos);
	void set_visibility(unsigned int, bool);

private:
	std::array<glm::vec2, 4> player_positions;
	//std::array<glm::vec2, 4> player_vector;
	std::array<bool, 4> is_invisible;
	const unsigned int stage_width{ 28 };
	const unsigned int screen_height{ 16 };
};

class UserInterface
{
public:
	
	UserInterface();

	void update(const std::vector<Model> &models, int players, glm::vec3 &camera_pos);
	void render(const Shader &shader)const;
	void disable_chat();
	void enable_chat();
	PlayerArrows player_arrows;
protected:

	void rebind_buffers();

	VertexArray vao;
	Buffer vertex_buffer;
	Buffer gui_buffer;
	std::array<Texture, 10> gui_textures;
	unsigned int active_texture;
	std::array<GuiElement, 100> elements;
};

}

#endif
