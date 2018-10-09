#ifndef INTROSPECTION_HPP_INCLUDED
#define INTROSPECTION_HPP_INCLUDED

#include <string>
#include <SDL2/SDL_render.h>

class Introspection
{
	private:
		static int _numRenderDrivers;
		static int _numVideoDisplays;
		static int _numVideoDrivers;
		static int _numAudioDrivers;
		static int _numAudioDevices;
		static std::string _currentVideoDriver;
		static std::string _currentAudioDriver;

		static void videoDisplays(void);
		static void audioDevices(void);
		static void videoDrivers(void);
		static void audioDrivers(void);
		static void renderDrivers(void);
		static void renderDriverInfo(SDL_RendererInfo const & info);

	public:
		static void perform(void);
		static void logRendererInfo(SDL_Renderer * renderer);
};

#endif // INTROSPECTION_HPP_INCLUDED
