#pragma once
#include <vector>

struct Counter
{
	unsigned int vertexCount;
	unsigned int meshCount;
	unsigned int customMayaAttributeCount;
	unsigned int matCount;
	unsigned int levelObjectCount;
};

struct MeshInfo {
	char meshName[100];
	float globalTranslation[3];
	float globalRotation[3];
	float globalScaling[3];
};

struct VertexInformation {
	float x, y, z;
	float nx, ny, nz;
	float bnx, bny, bnz;
	float tx, ty, tz;
	float u, v;
	float weight[4];
	float weightID[4];
};

struct MaterialInformation {
	float ambient[3];
	float diffuse[3];
	float emissive[3];
	float opacity;
	char textureName[100];
};

struct CustomMayaAttributes {
	float particlePivot[3];
	int id;
};

struct LevelObject {
	float position[3];
	float rotation[3];
	float centerPivot[3];
	float collisionBox[2];
	int id;
};