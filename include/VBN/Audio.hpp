#ifndef AUDIO_HPP_INCLUDED
#define AUDIO_HPP_INCLUDED

#include <array>
#include <SDL2/SDL_audio.h>


class Audio
{
	public:
		struct Sample;
		static Sample _sounds;
		static SDL_AudioDeviceID _device;
		static SDL_AudioSpec _wantedSpec;
		static SDL_AudioSpec _currentSpec;

		static void init(void);
		static void mixaudio(void *unused, Uint8 *stream, int length);
		static void playSound(char const * file);
		static void getStatus(void);
};

#endif // AUDIO_HPP_INCLUDED
