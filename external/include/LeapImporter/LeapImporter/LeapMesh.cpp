#include "LeapMesh.h"

LeapMesh::LeapMesh()
{
}

LeapMesh::LeapMesh(const char* fileName)
{
	loader(fileName);
}

LeapMesh::~LeapMesh()
{

}

int LeapMesh::getVertexCount()
{
	return this->counterReader.vertexCount;
}

void LeapMesh::loader(const char* fileName)
{
	
	std::ifstream infile(fileName, std::ifstream::binary);

	infile.read((char*)&counterReader, sizeof(Counter));

	transform = new MeshInfo[counterReader.meshCount];
	
	for (unsigned int i = 0; i < counterReader.meshCount; i++)
	{
		infile.read((char*)transform[i].meshName, sizeof(char) * 100);
		infile.read((char*)transform[i].globalTranslation, sizeof(float) * 3);
		infile.read((char*)transform[i].globalRotation, sizeof(float) * 3);
		infile.read((char*)transform[i].globalScaling, sizeof(float) * 3);
	}

	vertices = new VertexInformation[counterReader.vertexCount];
	infile.read((char*)vertices, counterReader.vertexCount * sizeof(VertexInformation));

	material = new MaterialInformation[counterReader.matCount];
	infile.read((char*)material, sizeof(MaterialInformation) * counterReader.matCount);

	animation = new AnimationInformation[1];

	if (animation->nrOfJoints > 0)
	{
		infile.read((char*)animation->animationName, sizeof(char) * 9);
		infile.read((char*)&animation->keyFrameCount, sizeof(int));
		infile.read((char*)&animation->nrOfJoints, sizeof(int));
		animation->joints.resize(animation->nrOfJoints);
		for (int i = 0; i < animation->nrOfJoints; i++)
		{	
			infile.read((char*)&animation->joints[i].jointName, sizeof(char) * 100);
			infile.read((char*)&animation->joints[i].parentName, sizeof(char) * 100);
			animation->joints[i].keyFrames.resize(animation->keyFrameCount);
			infile.read((char*)animation->joints[i].keyFrames.data(), sizeof(KeyFrame) * animation->keyFrameCount);
		}
	}

	customMayaAttribute = new CustomMayaAttributes[counterReader.customMayaAttributeCount];
	infile.read((char*)customMayaAttribute, sizeof(CustomMayaAttributes) * counterReader.customMayaAttributeCount);

	if (infile.is_open())
	{
		infile.close();
	}

	return;
}