#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <AL/al.h>
#include <AL/alc.h>

class Audio
{
public:
	Audio();
	~Audio();

private:
	ALCdevice* device;
	ALCcontext* context;


};


#endif
