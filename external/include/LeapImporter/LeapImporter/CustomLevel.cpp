#include "CustomLevel.h"

void CustomLevel::loader(const char * fileName)
{
	std::ifstream infile(fileName, std::ifstream::binary);

	infile.read((char*)&counterReader, sizeof(Counter));

	for (int i = 0; i < counterReader.levelObjectCount; i++)
	{
		LevelObject* lvl = new LevelObject();
		levelObjects.push_back(lvl);
		infile.read((char*)levelObjects[i], sizeof(LevelObject));
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
	for (LevelObject* lvl_ptr : levelObjects)
	{
		delete lvl_ptr;
	}
	levelObjects.clear();
}