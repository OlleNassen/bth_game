#ifndef MESH_HPP
#define MESH_HPP
#include "shader.hpp"
#include <string>
#include <GL/glew.h>
#include <LeapImporter/LeapImporter/LeapImporter.h>

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

class Mesh
{
public:
	Mesh();
	Mesh(const std::string meshfile);
	~Mesh();

	void render()const;

private:
	unsigned int vao_id;
	unsigned int vbo_id;

	LeapMesh* custom_mesh;
};

#endif