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
	int weightID[4];
};

struct MaterialInformation {
	float ambient[3];
	float diffuse[3];
	float emissive[3];
	float opacity;
	char textureFilePath[100];
};

struct KeyFrame {
	float time;
	float position[3];
	float rotation[3];
	float scaling[3];
};

struct JointInformation {
	char jointName[100];
	char parentName[100];
	std::vector<KeyFrame> keyFrames;
};

struct AnimationInformation {
	char animationName[9];
	int keyFrameCount;
	int nrOfJoints;
	std::vector<JointInformation> joints;
};

struct CustomMayaAttributes {
	float particlePivot[3];
	float centerPivot[3];
	float height;
	float width;
	int id;
};

struct LevelObject {
	float x, y, z;
	float rotationX, rotationY, rotationZ;
	int id;
};