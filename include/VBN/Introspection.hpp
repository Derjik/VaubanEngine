#ifndef INTROSPECTION_HPP_INCLUDED
#define INTROSPECTION_HPP_INCLUDED

#include <string>
#include <sstream>
#include <SDL2/SDL_render.h>

class Introspection
{
	private:
		static std::stringstream _synthesis;

		static int _numRenderDrivers;
		static int _numVideoDisplays;
		static int _numVideoDrivers;
		static int _numAudioDrivers;
		static int _numAudioDevices;
		static std::string _currentVideoDriver;
		static std::string _currentAudioDriver;

		static void fetchVideoDisplays(void);
		static void fetchAudioDevices(void);
		static void fetchVideoDrivers(void);
		static void fetchAudioDrivers(void);
		static void fetchRenderDrivers(void);
		static void fetchRenderDriverInfo(SDL_RendererInfo const & info);

	public:
		static void perform(void);
		static void log(void);
		static void insertRendererInfo(SDL_Renderer * renderer);
};

#endif // INTROSPECTION_HPP_INCLUDED
