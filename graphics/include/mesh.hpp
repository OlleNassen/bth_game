#ifndef MESH_HPP
#define MESH_HPP
#include "shader.hpp"
#include <string>
#include <GL/glew.h>
#include <LeapImporter/LeapImporter/LeapImporter.h>
#include "helpers.hpp"

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

namespace graphics
{

class Mesh
{
public:
	Mesh();
	Mesh(const std::string& meshfile);
	~Mesh();
	std::string name;
	glm::vec3 center_pivot;
	float height;
	float width;

	void render()const;

private:
	VertexArray vao_id;
	Buffer vbo_id;	

	LeapMesh* custom_mesh;
};

}

#endif