#include <VBN/Mixer.hpp>
#include <VBN/Logging.hpp>
#include <sstream>

Mixer::Mixer(unsigned int const deviceIndex) : _deviceIndex(deviceIndex)
{
	if (_deviceIndex < 0)
		THROW(Exception,
			"Received 'deviceIndex' < 0");
	if (_deviceIndex >= SDL_GetNumAudioDevices(0))
		THROW(Exception,
			"Received 'deviceIndex' > %d",
			SDL_GetNumAudioDevices(0) - 1);

	if (Mix_OpenAudioDevice(MIX_DEFAULT_FREQUENCY,
							MIX_DEFAULT_FORMAT,
							MIX_DEFAULT_CHANNELS,
							4096,
							SDL_GetAudioDeviceName(_deviceIndex, 0),
							SDL_AUDIO_ALLOW_FORMAT_CHANGE))
		THROW(Exception,
			"Cannot open audio device '%s' : Mix error '%s'",
			SDL_GetAudioDeviceName(_deviceIndex, 0),
			Mix_GetError());
}

Mixer::~Mixer(void)
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Delete Mixer %p (device index %d)",
		this,
		_deviceIndex);

	_effects.clear();
	_musics.clear();
	Mix_CloseAudio();
}

void Mixer::loadEffect(std::string const & path,
						std::string const & name)
{
	if (path.empty())
		THROW(Exception, "Received empty 'path'");
	if (name.empty())
		THROW(Exception, "Received empty 'name'");

	Mix_Chunk * rawEffect(Mix_LoadWAV(path.c_str()));
	if (rawEffect == nullptr)
		THROW(Exception,
			"Cannot load '%s' : Mix error '%s'",
			path.c_str(),
			Mix_GetError());

	_effects.emplace(
		std::make_pair(
			name,
			std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>(
				rawEffect, &Mix_FreeChunk)));
}

void Mixer::playEffect(std::string const & name)
{
	if (_effects.find(name) == _effects.end())
		THROW(Exception, "No effect '%s' loaded", name.c_str());

	if (Mix_PlayChannel(-1, _effects.at(name).get(), 0))
		ERROR(SDL_LOG_CATEGORY_AUDIO,
			"Cannot play effect '%s' : Mix error '%s'",
			name.c_str(),
			Mix_GetError());
}

void Mixer::loadMusic(std::string const & path,
						std::string const & name)
{
	if (path.empty())
		THROW(Exception, "Received empty 'path'");
	if (name.empty())
		THROW(Exception, "Received empty 'name'");

	Mix_Music * rawMusic(Mix_LoadMUS(path.c_str()));
	if (rawMusic == nullptr)
		THROW(Exception,
			"Cannot load '%s' : Mix error '%s'",
			path.c_str(),
			Mix_GetError());

	_musics.emplace(
		std::make_pair(
			name,
			std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)>(
				rawMusic, &Mix_FreeMusic)));
}

void Mixer::playMusic(std::string const & name)
{
	if (_musics.find(name) == _musics.end())
		THROW(Exception, "No music '%s' loaded", name.c_str());

	if(Mix_PlayMusic(_musics.at(name).get(), 0))
		ERROR(SDL_LOG_CATEGORY_AUDIO,
			"Cannot play music '%s' : Mix error '%s'",
			name.c_str(),
			Mix_GetError());
}
