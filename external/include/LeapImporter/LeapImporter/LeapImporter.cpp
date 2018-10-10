#include "LeapImporter.h"

LeapImporter::LeapImporter()
{
}

LeapImporter::~LeapImporter()
{

}

LeapMesh * LeapImporter::getMesh(const char* meshName)
{
	//Check file type and get error if not correct.

	LeapMesh* newMesh = new LeapMesh(meshName);

	return newMesh;
}

LeapAnimation * LeapImporter::getAnimation(const char * meshName)
{
	LeapAnimation* newAnimation = new LeapAnimation(meshName);
	return newAnimation;
}

CustomLevel * LeapImporter::getLevel(const char * levelName)
{

	CustomLevel* newLevel = new CustomLevel(levelName);

	return newLevel;
}

void LeapImporter::deleteObject(LeapMesh * mesh)
{
	delete mesh;
}

void LeapImporter::deleteObject(CustomLevel * level)
{
	delete level;
}

void LeapImporter::deleteObject(LeapAnimation * animation)
{
	delete animation;
}
