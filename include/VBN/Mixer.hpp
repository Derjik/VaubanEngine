#ifndef MIXER_HPP_INCLUDED
#define MIXER_HPP_INCLUDED

#include <map>
#include <string>
#include <memory>
#include <SDL2/SDL_mixer.h>

class Mixer
{
	public:
		typedef std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)> Chunk;
		typedef std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)> Music;

	private:
		// SDL Mixer Device ID
		int _deviceIndex;

		// Full or relative path to Audio Assets Directory
		std::string _assetsDirectory;

		// std::string-indexed list of sound effects
		std::map<std::string, Chunk> _effects;
		// std::string-inexed list of music tracks
		std::map<std::string, Music> _musics;

	public:
		/* Constructors & destructors */
		// May throw
		Mixer(unsigned int constdeviceIndex,
			std::string const & assetsDirectory,
			std::map<std::string, std::string> const & effects,
			std::map<std::string, std::string> const & musics);
		Mixer(Mixer const &) = delete;
		Mixer(Mixer const &&) = delete;
		Mixer & operator = (Mixer const &) = delete;
		Mixer & operator = (Mixer const &&) = delete;
		~Mixer(void);

		/* Loading methods */
		// May throw
		void loadEffect(std::string const & assetFile,
						std::string const & effectName);
		// May throw
		void loadMusic(std::string const & assetFile,
						std::string const & musicName);

		/* Playback methods */
		// May throw
		void playEffect(std::string const & effectName);
		// May throw
		void playMusic(std::string const & musicName);
};

#endif // MIXER_HPP_INCLUDED
