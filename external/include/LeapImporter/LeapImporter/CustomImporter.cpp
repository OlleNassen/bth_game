#include "CustomImporter.h"

CustomImporter::CustomImporter()
{
}

CustomImporter::~CustomImporter()
{

}

CustomMesh * CustomImporter::getMesh(const char* meshName)
{
	//Check file type and get error if not correct.

	CustomMesh* newMesh = new CustomMesh(meshName);

	return newMesh;
}

void CustomImporter::deleteObject(CustomMesh * mesh)
{
	delete mesh;
}
