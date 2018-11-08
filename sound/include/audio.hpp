#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <fstream>
#include <AL/al.h>
#include <AL/alc.h>
#include <FLAC/all.h>

class Audio
{
public:
	Audio();

	struct ClientData
	{
		InputStream*          stream;
		SoundFileReader::Info info;
		Int16*                buffer;
		Uint64                remaining;
		std::vector<Int16>    leftovers;
		bool                  error;
	};

private:
	ALCdevice* device;
	ALCcontext* context;

	ALuint buffer;

	FLAC__StreamDecoder* decoder;
	ClientData client_data;

};


#endif
