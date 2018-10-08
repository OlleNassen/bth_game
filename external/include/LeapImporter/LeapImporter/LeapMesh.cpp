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

	customMayaAttribute = new CustomMayaAttributes[counterReader.customMayaAttributeCount];
	infile.read((char*)customMayaAttribute, sizeof(CustomMayaAttributes) * counterReader.customMayaAttributeCount);

	if (infile.is_open())
	{
		infile.close();
	}

	return;
}