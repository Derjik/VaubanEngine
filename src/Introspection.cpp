#include <VBN/Introspection.hpp>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_log.h>

int Introspection::_numRenderDrivers;
int Introspection::_numVideoDisplays;
int Introspection::_numVideoDrivers;
int Introspection::_numAudioDevices;
int Introspection::_numAudioDrivers;
std::string Introspection::_currentVideoDriver;
std::string Introspection::_currentAudioDriver;

void Introspection::videoDisplays(void)
{
	_numVideoDisplays = SDL_GetNumVideoDisplays();

	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Video displays : %d",
		_numVideoDisplays);
	SDL_DisplayMode displayMode;
	for(int displayIndex(0) ;
		displayIndex < _numVideoDisplays ;
		++displayIndex)
	{
		SDL_GetCurrentDisplayMode(displayIndex, &displayMode);
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
			"\t'%s' %dx%d@%dHz %s",
			SDL_GetDisplayName(displayIndex),
			displayMode.w, displayMode.h,
			displayMode.refresh_rate,
			SDL_GetPixelFormatName(displayMode.format));
	}
}

void Introspection::audioDevices(void)
{
	_numAudioDevices = SDL_GetNumAudioDevices(0);
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
		"Audio devices : %d", _numAudioDevices);
	for(int deviceIndex(0) ;
		deviceIndex < _numAudioDevices ;
		++deviceIndex)
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
			"\t'%s'", SDL_GetAudioDeviceName(deviceIndex, 0));
}

void Introspection::videoDrivers(void)
{
	std::string prefix, suffix;

	_numVideoDrivers = SDL_GetNumVideoDrivers();
	_currentVideoDriver = SDL_GetCurrentVideoDriver();
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
		"Video drivers : %d", _numVideoDrivers);
	for(int driverIndex(0) ;
		driverIndex < _numVideoDrivers ;
		++driverIndex)
	{
		if(std::strcmp(SDL_GetVideoDriver(driverIndex),
			_currentVideoDriver.c_str()))
		{
			prefix = " ";
			suffix = "";
		}
		else
		{
			prefix = "[";
			suffix = "]";
		}
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
			" %s'%s'%s",
			prefix.c_str(),
			SDL_GetVideoDriver(driverIndex),
			suffix.c_str());
	}
}

void Introspection::audioDrivers(void)
{
	std::string prefix, suffix;

	_numAudioDrivers = SDL_GetNumAudioDrivers();
	_currentAudioDriver = SDL_GetCurrentAudioDriver();
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
		"Audio drivers : %d", _numAudioDrivers);
	for(int driverIndex(0) ;
		driverIndex < _numAudioDrivers ;
		++driverIndex)
	{
		if(std::strcmp(SDL_GetAudioDriver(driverIndex),
			_currentAudioDriver.c_str()))
		{
			prefix = " ";
			suffix = "";
		}
		else
		{
			prefix = "[";
			suffix = "]";
		}
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
			" %s'%s'%s",
			prefix.c_str(),
			SDL_GetAudioDriver(driverIndex),
			suffix.c_str());
	}

}

void Introspection::renderDriverInfo(SDL_RendererInfo const & rendererInfo)
{
	std::string flags;
	if(SDL_RENDERER_ACCELERATED & rendererInfo.flags)
		flags += "A";
	if(SDL_RENDERER_PRESENTVSYNC & rendererInfo.flags)
		flags += "V";
	if(SDL_RENDERER_SOFTWARE & rendererInfo.flags)
		flags += "S";
	if(SDL_RENDERER_TARGETTEXTURE & rendererInfo.flags)
		flags += "T";

	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
		"\t'%s' [%s] [%dx%d] has %d pixel formats :",
		rendererInfo.name,
		flags.c_str(),
		rendererInfo.max_texture_width,
		rendererInfo.max_texture_height,
		rendererInfo.num_texture_formats);

	for(int formatIndex(0) ;
		formatIndex < rendererInfo.num_texture_formats ;
		++formatIndex)
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "\t\t%s",
			SDL_GetPixelFormatName(
				rendererInfo.texture_formats[formatIndex]));
}

void Introspection::renderDrivers(void)
{
	_numRenderDrivers = SDL_GetNumRenderDrivers();

	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
		"Render drivers : %d", _numRenderDrivers);
	for(int driverIndex(0) ;
		driverIndex < _numRenderDrivers ;
		++driverIndex)
	{
		SDL_RendererInfo rendererInfo;
		SDL_GetRenderDriverInfo(driverIndex, &rendererInfo);
		renderDriverInfo(rendererInfo);
	}

}

void Introspection::perform(void)
{
	videoDisplays();
	audioDevices();
	videoDrivers();
	audioDrivers();
	renderDrivers();
}

void Introspection::logRendererInfo(SDL_Renderer * renderer)
{
	SDL_RendererInfo rendererInfo;
	SDL_GetRendererInfo(renderer, &rendererInfo);
	renderDriverInfo(rendererInfo);
}