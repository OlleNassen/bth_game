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

	void init_irradiance()const
	{
		glBindVertexArray(vao_id);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	void irradiance_render(const Shader& shader, const Camera& camera)const
	{
		shader.uniform("skybox", 8);
		shader.uniform("view", glm::mat4(glm::mat3(camera.view())));
		shader.uniform("projection", camera.projection);

		glDepthFunc(GL_LEQUAL);
		glBindVertexArray(vao_id);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);
	}

	void bind_texture(int offset = 0)const
	{
		texture.bind(offset, GL_TEXTURE_CUBE_MAP);
	}

private:
	unsigned int vao_id;
	unsigned int vbo_id;

	SkyboxTexture texture;
};

}


#endif