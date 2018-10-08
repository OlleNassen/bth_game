#ifndef STUPID_FILE_HPP
#define STUPID_FILE_HPP
#include <gl/glew.h>
#include <glm/glm.hpp>
#include "shader.hpp"
#include "camera.hpp"
#include "texture.hpp"

namespace graphics
{

class Temp
{
public:
	Temp()
	{
		// positions
		glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
		glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
		glm::vec3 pos3(1.0f, -1.0f, 0.0f);
		glm::vec3 pos4(1.0f, 1.0f, 0.0f);
		// texture coordinates
		glm::vec2 uv1(0.0f, 1.0f);
		glm::vec2 uv2(0.0f, 0.0f);
		glm::vec2 uv3(1.0f, 0.0f);
		glm::vec2 uv4(1.0f, 1.0f);
		// normal vector
		glm::vec3 nm(0.0f, 0.0f, 1.0f);

		// calculate tangent/bitangent vectors of both triangles
		glm::vec3 tangent1;
		glm::vec3 tangent2;
		// triangle 1
		// ----------
		auto edge1 = pos2 - pos1;
		auto edge2 = pos3 - pos1;
		auto deltaUV1 = uv2 - uv1;
		auto deltaUV2 = uv3 - uv1;

		auto f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent1 = glm::normalize(tangent1);

		// triangle 2
		// ----------
		edge1 = pos3 - pos1;
		edge2 = pos4 - pos1;
		deltaUV1 = uv3 - uv1;
		deltaUV2 = uv4 - uv1;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent2 = glm::normalize(tangent2);

		float quad_vertices[] = {
			// positions  // normal // texcoords  // tangent
			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y,
			tangent1.x, tangent1.y, tangent1.z,

			pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y,
			tangent1.x, tangent1.y, tangent1.z,

			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y,
			tangent1.x, tangent1.y, tangent1.z,

			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y,
			tangent2.x, tangent2.y, tangent2.z,

			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y,
			tangent2.x, tangent2.y, tangent2.z,

			pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y,
			tangent2.x, tangent2.y, tangent2.z,
		};

		auto stride = 11 * sizeof(float);


		glGenVertexArrays(1, &vao_id);
		glBindVertexArray(vao_id);

		glGenBuffers(1, &vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices),
			&quad_vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride,
			nullptr);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
			(void*)(sizeof(float) * 3));

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride,
			(void*)(sizeof(float) * 6));

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride,
			(void*)(sizeof(float) * 8));

		textures.reserve(sizeof(Texture) * 3);
		textures.emplace_back("../resources/assets/brickwall.jpg");
		textures.emplace_back("../resources/assets/brickwall_normal.jpg");
	}

	void render(const Shader & shader, const DebugCamera& camera)const
	{
		shader.uniform("model", glm::mat4(1.f));
		shader.uniform("view", camera.view());
		shader.uniform("projection", camera.projection);

		shader.uniform("view_position", glm::vec3{ camera.position });

		static glm::vec3 light_pos = glm::vec3(-15, -2, 6);
		light_pos.x += glm::sin(glfwGetTime() * 2.0f);
		light_pos.y += glm::sin(glfwGetTime() * 0.7f);


		shader.uniform("light_position", light_pos);

		shader.uniform("diffuse_map", 0);
		shader.uniform("normal_map", 1);

		textures[0].bind(0);
		textures[1].bind(1);

		glBindVertexArray(vao_id);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

private:
	unsigned int vao_id;
	unsigned int vbo_id;
	glm::mat4 model = { glm::mat4(1.f) };
	std::vector<Texture>textures;

};

}

#endif