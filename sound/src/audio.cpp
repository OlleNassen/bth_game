#include "audio.hpp"

Audio::Audio()
{
	std::ifstream in{"../resources/sounds/test.ogg"};

	
	
	device = alcOpenDevice(nullptr);

	if (device)
	{
		context = alcCreateContext(device, nullptr);
		alcMakeContextCurrent(context);
	}

	alGenBuffers(1, &buffer);

	int channels;
	int sample_rate;
	short* output;

	/*stb_vorbis* stream;
	stream = stb_vorbis_open_filename("test.ogg", nullptr, nullptr);
	stb_vorbis_info stream_info = stb_vorbis_get_info(stream);
	stb_vorbis_stream_length_in_samples*/

	alBufferData(buffer, AL_FORMAT_MONO16, output, 4096, sample_rate);

	alSourcePlay(buffer);
}