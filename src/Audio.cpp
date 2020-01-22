#include <VBN/Audio.hpp>
#include <VBN/Logging.hpp>
#define NUM_SOUNDS 2

struct Audio::Sample
{
	Uint8 *data;
	Uint32 dataPosition;
	Uint32 dataLength;
};

Audio::Sample Audio::_sounds;
SDL_AudioDeviceID Audio::_device;
SDL_AudioSpec Audio::_wantedSpec;
SDL_AudioSpec Audio::_currentSpec;

void Audio::init(void)
{
	_wantedSpec.freq = 48000;
	_wantedSpec.format = AUDIO_F32;
	_wantedSpec.channels = 2;
	_wantedSpec.samples = 4096;
	_wantedSpec.callback = Audio::mixaudio;
	_wantedSpec.userdata = NULL;

	if ((_device = SDL_OpenAudioDevice(
		SDL_GetAudioDeviceName(0, 0),
		0,
		&_wantedSpec,
		&_currentSpec,
		SDL_AUDIO_ALLOW_FORMAT_CHANGE)) < 0)
		ERROR(SDL_LOG_CATEGORY_AUDIO, "Cannot open audio : %s", SDL_GetError());
	else
		SDL_PauseAudioDevice(_device, 0);
}

void Audio::mixaudio(void *udata, Uint8 * stream, int length)
{
	SDL_memset(stream, 0, length);
	if (_sounds.data && _sounds.dataPosition < _sounds.dataLength)
	{
		int toPlay = _sounds.dataLength - _sounds.dataPosition;
		if (toPlay < length)
			length = toPlay;

		DEBUG(SDL_LOG_CATEGORY_AUDIO, "Position : %d ; Length : %d ; Add %d bytes", _sounds.dataPosition, _sounds.dataLength, length);

		SDL_MixAudioFormat(stream,
							&_sounds.data[_sounds.dataPosition],
							_currentSpec.format,
							length,
							SDL_MIX_MAXVOLUME);
		_sounds.dataPosition += length;
	}
}

void Audio::playSound(char const * file)
{
	DEBUG(SDL_LOG_CATEGORY_AUDIO, "Play %s", file);
	SDL_AudioSpec inputWave;
	Uint8 * audioData;
	Uint32 dataLength;

	if (SDL_LoadWAV(file, &inputWave, &audioData, &dataLength) == NULL) {
		ERROR(SDL_LOG_CATEGORY_AUDIO,
				"Couldn't load WAV file %s : %s",
				file,
				SDL_GetError());
		return;
	}

	SDL_AudioCVT converter;
	SDL_BuildAudioCVT(
		&converter,
		inputWave.format,
		inputWave.channels,
		inputWave.freq,
		_currentSpec.format,
		_currentSpec.channels,
		_currentSpec.freq);

	converter.buf = (Uint8*)(malloc(dataLength*converter.len_mult));
	memcpy(converter.buf, audioData, dataLength);
	converter.len = dataLength;
	SDL_ConvertAudio(&converter);

	SDL_FreeWAV(audioData);

	SDL_LockAudioDevice(_device);
	_sounds.data = converter.buf;
	_sounds.dataLength = converter.len_cvt;
	_sounds.dataPosition = 0;
	SDL_UnlockAudioDevice(_device);
	DEBUG(SDL_LOG_CATEGORY_AUDIO, "Added %d bytes to queue", converter.len_cvt);
}

void Audio::getStatus(void)
{
	std::string status;
	switch (SDL_GetAudioDeviceStatus(_device))
	{
		case SDL_AUDIO_PLAYING:
			status = "PLAY";
		break;
		case SDL_AUDIO_PAUSED:
			status = "PAUSE";
		break;
		case SDL_AUDIO_STOPPED:
			status = "STOP";
		break;
		default:
			status = "UNKNOWN";
		break;
	}
	DEBUG(SDL_LOG_CATEGORY_AUDIO,
		"Audio device %d status : %s",
		_device, status.c_str());
}