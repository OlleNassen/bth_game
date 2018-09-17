#include "LeapLevel.h"



void LeapLevel::loader(const char * fileName)
{
	/*std::ifstream infile(fileName, std::ifstream::binary);

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
	}*/

	return;
}

LeapLevel::LeapLevel()
{

}

LeapLevel::LeapLevel(const char * fileName)
{
	loader(fileName);
}


LeapLevel::~LeapLevel()
{
	for (LevelObject* lvl_ptr : levelObjects)
	{
		delete lvl_ptr;
	}
	levelObjects.clear();
}
