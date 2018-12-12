#include <laser.hpp>

namespace graphics
{

Laser::Laser()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(primitives::quad_uv), &primitives::quad_uv[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), nullptr);

	laser_textures.at(0).load_texture("../resources/textures/laser/middle_background_texture.png", true);
	//gui_textures.at(1).load_texture("../resources/textures/chat_texture.png", true);
}


void Laser::update()
{
	//Rebind buffers here

	//Do update stuff here
}

void Laser::render(const Shader &shader, const Camera& cam)const
{
	shader.use();
	glm::mat4 model(1.f);
	model[3][1] = 10.f;
	shader.uniform("model", model);
	shader.uniform("view", cam.view());
	shader.uniform("projection", cam.projection);
	shader.uniform("middle_laser", 0);

	laser.bind(0);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

}