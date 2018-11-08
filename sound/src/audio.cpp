#include "audio.hpp"

#define STB_VORBIS_HEADER_ONLY
#include <stb_vorbis.h>

Audio::Audio()
{
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

	stb_vorbis_decode_filename("../resources/sounds/test.ogg", &channels, &sample_rate, &output);
	alBufferData(buffer, AL_FORMAT_MONO16, output, 4096, sample_rate);

	alSourcePlay(buffer);
}