#ifndef SHADER_HPP
#define SHADER_HPP
#include <string>
#include <vector>
#include <array>
#include <gl/glew.h>
#include <glm/glm.hpp>

#include "light_grid.hpp"

//::.. authors ..:://
// Olle
// Edvard

namespace graphics
{

class Shader
{
public:
	Shader(const std::string& vertex_path, const std::string& fragment_path);

	Shader(const std::string& vertex_path,
		const std::string& geo_path, const std::string& fragment_path);

	~Shader();

	void use() const;

	void uniform(const std::string& name, int value) const;
	void uniform(const std::string& name, float value) const;
	void uniform(const std::string& name, const glm::vec2& value) const;
	void uniform(const std::string& name, const glm::vec3& value) const;
	void uniform(const std::string& name, const glm::vec4& value) const;
	void uniform(const std::string& name, const glm::ivec4& value) const;
	void uniform(const std::string& name, const glm::mat4& value) const;

	void uniform(const std::string& name,
		const std::vector<int>& value) const;
	void uniform(const std::string& name,
		const std::vector<float>& value) const;
	void uniform(const std::string& name,
		const std::vector<glm::vec2>& value) const;
	void uniform(const std::string& name,
		const std::vector<glm::vec3>& value) const;
	void uniform(const std::string& name,
		const std::vector<glm::vec4>& value) const;
	void uniform(const std::string& name,
		const std::vector<glm::mat4>& value) const;
	
	template <std::size_t N>
	void uniform(const std::string& name,
		const std::array<glm::mat4, N>& value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()),
			value.size(), GL_FALSE, glm::value_ptr(value.front()));
	}

private:
	unsigned int id;

	std::string load(const std::string& path) const;
	unsigned int create(unsigned int shader_type,
		const char* shader_code) const;
};

}

#endif // SHADER_HPP