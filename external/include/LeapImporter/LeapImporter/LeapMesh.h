#pragma once
#include "MeshStructs.h"
#include <vector>
#include <string>
#include <fstream>

class LeapMesh
{
private:
	void loader(const char* fileName);

public:
	LeapMesh();
	LeapMesh(const char* fileName);
	~LeapMesh();

	int getVertexCount();

	Counter counterReader;
	MeshInfo* transform;
	VertexInformation* vertices;
	MaterialInformation* material;
	CustomMayaAttributes* customMayaAttribute;
};