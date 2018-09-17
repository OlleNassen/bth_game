#pragma once

#include <fstream>
#include "MeshStructs.h"


class CustomMesh
{
private:
	void loader(const char* fileName);

public:
	CustomMesh() = default;
	CustomMesh(const char* fileName);
	~CustomMesh();

	int getVertexCount();

	Counter counterReader;
	MeshInfo* transform;
	VertexInformation* vertices;
	MaterialInformation* material;
	JointInformation* joints;
	AnimationInformation* animation;
	KeyFrame* keyFrame;
	CustomMayaAttributes* customMayaAttribute;
};