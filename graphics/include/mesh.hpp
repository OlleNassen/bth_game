#ifndef MESH_HPP
#define MESH_HPP
#include "shader.hpp"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void render(const Shader& shader);
private:
	unsigned int vao_id;
	unsigned int vbo_id;

};

#endif