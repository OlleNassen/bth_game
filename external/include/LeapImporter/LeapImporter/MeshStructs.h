#pragma once
#include <vector>
using namespace std;

struct Counter
{
	unsigned int vertexCount;
	unsigned int meshCount;
	unsigned int customMayaAttributeCount;
	unsigned int matCount;
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
	vector<KeyFrame> keyFrames;
};

struct AnimationInformation {
	char animationName[9];
	int keyFrameCount;
	int nrOfJoints;
	vector<JointInformation> joints;
};

//struct BlendShapeVertex {
//	float x, y, z;
//	float nx, ny, nz;
//};
//
//struct BlendShape
//{
//	int blendShapeVertexCount;
//	vector <BlendShapeVertex> blendShapeVertices;
//};
//
//struct BlendShapeKeyframe
//{
//	float time;
//	float blendShapeInfluense;
//};
//
//struct BlendShapes {
//	float blendShapeCount;
//	float keyFrameCount;
//	vector <BlendShape> blendShape;
//	vector <BlendShapeKeyframe> keyframes;
//};
//
//struct GroupChild {
//	char childName[100];
//};
//
//struct Group {
//	char groupName[100];
//	int childCount;
//	vector<GroupChild> children;
//};

struct CustomMayaAttributes {
	float particlePivot[3];
};

struct LevelObject {
	float x, y, z;
	float rotationX, rotationY, rotationZ;
	int id;
};

//struct Light {
//	char type;
//	float color[3];
//	float intensity;
//	float innerCone;
//	float outerCone;
//};
//
//struct Camera {
//	float position[3];
//	float up[3];
//	float forward[3];
//	float roll;
//	float aspectWidth;
//	float aspectHeight;
//	float fov;
//	float nearPlane;
//	float farPlane;
//};