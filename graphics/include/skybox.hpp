#ifndef SKYBOX_HPP
#define SKYBOX_HPP
#include "texture.hpp"
#include "shader.hpp"
#include "camera.hpp"

namespace graphics
{

class Skybox
{
public:
	Skybox();
	~Skybox() = default;

	void render(const Shader& shader, const Camera& camera)const;

	void temp_render()const
	{
		glBindVertexArray(vao_id);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	void temp_render2(const Shader& shader, const Camera& camera)const
	{
		shader.uniform("skybox", 0);
		shader.uniform("view", glm::mat4(glm::mat3(camera.view())));
		shader.uniform("projection", camera.projection);

		glDepthFunc(GL_LEQUAL);
		glBindVertexArray(vao_id);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	}

	void bind_texture()const
	{
		texture.bind(5, GL_TEXTURE_CUBE_MAP);
	}
	SkyboxTexture texture;
private:
	unsigned int vao_id;
	unsigned int vbo_id;
};

}


#endif