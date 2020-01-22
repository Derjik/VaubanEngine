#ifndef MIXER_HPP_INCLUDED
#define MIXER_HPP_INCLUDED

#include <map>
#include <string>
#include <memory>
#include <SDL2/SDL_mixer.h>

class Mixer
{
	private:
		int _deviceIndex;
		std::map<std::string,
				std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>>
				_effects;
		std::map<std::string,
				std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)>>
				_musics;

	public:
		// May throw
		Mixer(unsigned int const deviceIndex);
		Mixer(Mixer const &) = delete;
		Mixer(Mixer const &&) = delete;
		Mixer & operator = (Mixer const &) = delete;
		Mixer & operator = (Mixer const &&) = delete;
		~Mixer(void);

		// May throw
		void loadEffect(std::string const & path,
						std::string const & name);

		// May throw
		void playEffect(std::string const & name);

		// May throw
		void loadMusic(std::string const & path,
						std::string const & name);

		// May throw
		void playMusic(std::string const & name);
};

#endif // MIXER_HPP_INCLUDED
