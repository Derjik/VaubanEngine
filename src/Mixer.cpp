#include <VBN/Mixer.hpp>
#include <VBN/Logging.hpp>
#include <sstream>

Mixer::Mixer(unsigned int const deviceIndex,
	std::string const & assetsDirectory,
	std::map<std::string, std::string> const & effects,
	std::map<std::string, std::string> const & musics) :
	_deviceIndex(deviceIndex),
	_assetsDirectory(assetsDirectory)
{
	/* Check deviceIndex boundaries */
	if (_deviceIndex < 0)
		THROW(Exception,
			"Received 'deviceIndex' < 0");
	if (_deviceIndex >= SDL_GetNumAudioDevices(0))
		THROW(Exception,
			"Received 'deviceIndex' > %d",
			SDL_GetNumAudioDevices(0) - 1);

	/* Attempt audio device initialization */
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

	/* Load effects & musics from parameter dictionnaries */
	for (auto const & effectIterator : effects)
		loadEffect(effectIterator.first, effectIterator.second);
	for (auto const & musicIterator : musics)
		loadMusic(musicIterator.first, musicIterator.second);
}

Mixer::~Mixer(void)
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Delete Mixer %p (device index %d)",
		this,
		_deviceIndex);

	/* Delete all stored sound effects & music tracks */
	_effects.clear();
	_musics.clear();
	/* Close SDL_mixer audio facility */
	Mix_CloseAudio();
}

void Mixer::loadEffect(std::string const & assetFile,
						std::string const & effectName)
{
	if (assetFile.empty())
		THROW(Exception, "Received empty 'assetFile'");
	if (effectName.empty())
		THROW(Exception, "Received empty 'effectName'");

	std::string fullPath = _assetsDirectory + assetFile;

	Mix_Chunk * rawEffect(Mix_LoadWAV(fullPath.c_str()));
	if (rawEffect == nullptr)
		THROW(Exception,
			"Cannot load sound effect '%s' : Mix error '%s'",
			fullPath.c_str(),
			Mix_GetError());

	_effects.emplace(
		std::make_pair(
			effectName,
				Chunk(rawEffect, &Mix_FreeChunk)));
}

void Mixer::loadMusic(std::string const & assetFile,
						std::string const & musicName)
{
	if (assetFile.empty())
		THROW(Exception, "Received empty 'assetFile'");
	if (musicName.empty())
		THROW(Exception, "Received empty 'musicName'");

	std::string fullPath = _assetsDirectory + assetFile;

	Mix_Music * rawMusic(Mix_LoadMUS(fullPath.c_str()));
	if (rawMusic == nullptr)
		THROW(Exception,
			"Cannot load music track '%s' : Mix error '%s'",
			fullPath.c_str(),
			Mix_GetError());

	_musics.emplace(
		std::make_pair(
			musicName,
			Music(rawMusic, &Mix_FreeMusic)));
}

void Mixer::playEffect(std::string const & effectName)
{
	if (_effects.find(effectName) == _effects.end())
		THROW(Exception, "No sound effect '%s' loaded", effectName.c_str());

	if (Mix_PlayChannel(-1, _effects.at(effectName).get(), 0))
		ERROR(SDL_LOG_CATEGORY_AUDIO,
			"Cannot play effect '%s' : Mix error '%s'",
			effectName.c_str(),
			Mix_GetError());
}

void Mixer::playMusic(std::string const & musicName)
{
	if (_musics.find(musicName) == _musics.end())
		THROW(Exception, "No music track '%s' loaded", musicName.c_str());

	if(Mix_PlayMusic(_musics.at(musicName).get(), 0))
		ERROR(SDL_LOG_CATEGORY_AUDIO,
			"Cannot play music '%s' : Mix error '%s'",
			musicName.c_str(),
			Mix_GetError());
}
