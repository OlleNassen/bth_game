#include "audio.hpp"


Audio::Audio()
{
	device = alcOpenDevice(nullptr);

	if (device)
	{
		context = alcCreateContext(device, nullptr);
		alcMakeContextCurrent(context);
	}
}

Audio::~Audio()
{

}