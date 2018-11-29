#include "skybox.hpp"
#include <gl\glew.h>
#include "primitives.hpp"

namespace graphics
{

std::vector<std::string> faces
{
	/*
	"images/skybox3/petesoasis_rt.tga", // RIGHT
	"images/skybox3/petesoasis_lf.tga", // LEFT
	"images/skybox3/petesoasis_up.tga", // TOP
	"images/skybox3/petesoasis_dn.tga", // BOTTOM
	"images/skybox3/petesoasis_bk.tga", // BACK
	"images/skybox3/petesoasis_ft.tga" // FRONT
	*/
	/*
	"images/skybox3/negx.jpg", // RIGHT
	"images/skybox3/posx.jpg", // LEFT
	"images/skybox3/posy.jpg", // TOP
	"images/skybox3/negy.jpg", // BOTTOM
	"images/skybox3/negz.jpg", // BACK
	"images/skybox3/posz.jpg" // FRONT
	*/
	"../resources/textures/skybox2/black.png",	// FRONT
	"../resources/textures/skybox2/black.png",	// BACK
	"../resources/textures/skybox2/black.png",	// TOP
	"../resources/textures/skybox2/black.png",	// BOTTOM
	"../resources/textures/skybox2/black.png",	// RIGHT
	"../resources/textures/skybox2/black.png"	// LEFT
};

Skybox::Skybox()
	:texture(faces)
{
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);

	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

	glBufferData(
		GL_ARRAY_BUFFER, 36 * 3 * sizeof(float),
		&primitives::skybox, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

}

void Skybox::render(const Shader& shader, const Camera& camera) const
{
	shader.use();
	shader.uniform("skybox", 0);
	shader.uniform("view", glm::mat4(glm::mat3(camera.view())));
	shader.uniform("projection", camera.projection);


	texture.bind(0, GL_TEXTURE_CUBE_MAP);

	glDepthFunc(GL_LEQUAL);
	glBindVertexArray(vao_id);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);
}

}
