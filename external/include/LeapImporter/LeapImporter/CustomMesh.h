#pragma once
#include "MeshStructs.h"
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class CustomMesh
{
private:
	void loader(const char* fileName);

public:
	CustomMesh();
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