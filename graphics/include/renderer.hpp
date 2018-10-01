#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <chrono>
#include <vector>
#include "model.hpp"
#include "camera.hpp"
#include "text.hpp"
#include "framebuffer.hpp"
#include "post_processing_effects.hpp"
#include "user_interface.hpp"
#include "../../engine/include/config.hpp"
#include "../../engine/include/timer.hpp"
#include "../../engine/include/gui.hpp"

//Vincent nad Lucas
#include <../../physics/include/World.hpp> //Temp


//::.. authors ..:://
// Olle
// Edvard
static void line_debug(const std::vector<glm::vec2>& lines)
{
	VertexArray vao;
	Buffer vertex_buffer;

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	gl_buffer_data(GL_ARRAY_BUFFER, lines, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

	glLineWidth(1.0f);
	glDrawArrays(GL_LINES, 0, lines.size());
	glBindVertexArray(0);
}

class Renderer
{
public:
	Renderer();

	void render( const std::string* begin, const std::string* end, const gui::button_array& buttons, std::vector<glm::vec2> debug_positions) const;
	void update(std::chrono::milliseconds delta, const input* begin, const input* end,	const std::string& data, bool is_on, glm::vec2 position, bool &lua_jump, std::vector<glm::vec2> dynamic_pos);
	
private:
	DebugCamera db_camera;
	GameCamera game_camera;
	std::vector<Model> models;
	Text text;
	UserInterface ui;
	std::vector<Shader> shaders;
	std::vector<Mesh> meshes;
	std::chrono::milliseconds time{10000};

	//Win-clock
	std::chrono::milliseconds w_time{ 5000 };

	std::string log;
	Timer t;

	glm::vec2 v[4];
	bool is_chat_visible{false};

	bool game_over = false;
	bool game_win = false;
	bool game_reached_goal = false;

	bool show_start = false;


	Framebuffer scene_texture;
	PostProcessingEffects post_processing_effects;

	bool want_glow{false};
	 
	bool debug_active{ true };
	bool debug_camera_active{ false };
};

template <typename T>
void render_type(const Shader& shader, const Camera& camera, const T& data)
{
	shader.use();
	for (auto& renderable : data)
	{
		renderable.render(shader, camera);
	}
}

#endif