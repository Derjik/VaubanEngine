#include <VBN/Audio.hpp>
#include <VBN/Logging.hpp>
#define NUM_SOUNDS 2

struct Audio::Sample
{
	Uint8 *data;
	Uint32 dataPosition;
	Uint32 dataLength;
};

std::array<Audio::Sample, 2> Audio::_sounds;
SDL_AudioDeviceID Audio::_device;
SDL_AudioSpec Audio::_wantedSpec;
SDL_AudioSpec Audio::_currentSpec;

void Audio::init(void)
{
	SDL_AudioSpec wantedFormat, obtainedFormat;

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
	Uint32 amount;

	for (int i = 0; i < NUM_SOUNDS; ++i) {
		amount = (_sounds[i].dataLength - _sounds[i].dataPosition);

		if (amount > length)
			amount = length;

		SDL_MixAudioFormat(stream,
			&_sounds[i].data[_sounds[i].dataPosition],
			_currentSpec.format,
			amount,
			SDL_MIX_MAXVOLUME);

		_sounds[i].dataPosition += amount;
	}
}

void Audio::playSound(char const * file)
{
	int index;
	SDL_AudioSpec inputWave;
	Uint8 *audioData;
	Uint32 dataLength;
	SDL_AudioCVT converter;

	/* Look for an empty (or finished) sound slot */
	for (index = 0; index < NUM_SOUNDS; ++index) {
		if (_sounds[index].dataPosition == _sounds[index].dataLength) {
			break;
		}
	}
	if (index == NUM_SOUNDS)
		return;

	/* Load the sound file and convert it to 16-bit stereo at 22kHz */
	if (SDL_LoadWAV(file, &inputWave, &audioData, &dataLength) == NULL) {
		ERROR(SDL_LOG_CATEGORY_AUDIO,
				"Couldn't load WAV file %s : %s",
				file,
				SDL_GetError());
		return;
	}
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

	/* Put the sound data in the slot (it starts playing immediately) */
	if (_sounds[index].data) {
		free(_sounds[index].data);
	}
	SDL_LockAudioDevice(_device);
	_sounds[index].data = converter.buf;
	_sounds[index].dataLength = converter.len_cvt;
	_sounds[index].dataPosition = 0;
	SDL_UnlockAudioDevice(_device);
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