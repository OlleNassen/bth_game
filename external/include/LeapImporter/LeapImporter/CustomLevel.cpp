#include "CustomLevel.h"

void CustomLevel::loader(const char * fileName)
{
	std::ifstream infile(fileName, std::ifstream::binary);

	infile.read((char*)&counterReader, sizeof(Counter));

	levelObjects = new LevelObject[counterReader.levelObjectCount];

	for (unsigned int i = 0; i < counterReader.levelObjectCount; i++)
	{
		infile.read((char*)levelObjects[i].position, sizeof(float) * 3);
		infile.read((char*)levelObjects[i].rotation, sizeof(float) * 3);
		infile.read((char*)levelObjects[i].centerPivot, sizeof(float) * 3);
		infile.read((char*)levelObjects[i].collisionBox, sizeof(float) * 2);
		infile.read((char*)&levelObjects[i].id, sizeof(int));
	}
	if (infile.is_open())
	{
		infile.close();
	}

	return;
}

CustomLevel::CustomLevel()
{

}

CustomLevel::CustomLevel(const char * fileName)
{
	loader(fileName);
}

CustomLevel::~CustomLevel()
{
	
}