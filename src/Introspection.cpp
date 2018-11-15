#include <VBN/Introspection.hpp>
#include <VBN/Logging.hpp>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_audio.h>

std::stringstream Introspection::_synthesis;
int Introspection::_numRenderDrivers;
int Introspection::_numVideoDisplays;
int Introspection::_numVideoDrivers;
int Introspection::_numAudioDevices;
int Introspection::_numAudioDrivers;
std::string Introspection::_currentVideoDriver;
std::string Introspection::_currentAudioDriver;

void Introspection::fetchVideoDisplays(void)
{
	_numVideoDisplays = SDL_GetNumVideoDisplays();

	_synthesis << "Video displays : " << _numVideoDisplays << std::endl;

	SDL_DisplayMode displayMode;
	for(int displayIndex(0) ;
		displayIndex < _numVideoDisplays ;
		++displayIndex)
	{
		SDL_GetCurrentDisplayMode(displayIndex, &displayMode);
		_synthesis << "\t'" << SDL_GetDisplayName(displayIndex) << "'"
			<< " " << displayMode.w << "x" << displayMode.h
			<< " @ " << displayMode.refresh_rate << "Hz "
			<< SDL_GetPixelFormatName(displayMode.format) << std::endl;
	}
}

void Introspection::fetchAudioDevices(void)
{
	_numAudioDevices = SDL_GetNumAudioDevices(0);

	_synthesis << "Audio devices : " << _numAudioDevices << std::endl;

	for(int deviceIndex(0) ;
		deviceIndex < _numAudioDevices ;
		++deviceIndex)
			_synthesis << "\t'"
				<< SDL_GetAudioDeviceName(deviceIndex, 0)
				<< "'" << std::endl;
}

void Introspection::fetchVideoDrivers(void)
{
	std::string prefix, suffix;

	_numVideoDrivers = SDL_GetNumVideoDrivers();
	_currentVideoDriver = SDL_GetCurrentVideoDriver();

	_synthesis << "Video drivers : " << _numVideoDrivers << std::endl;

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
		_synthesis
			<< "\t" << prefix << "'"
			<< SDL_GetVideoDriver(driverIndex)
			<< "'" << suffix << std::endl;
	}
}

void Introspection::fetchAudioDrivers(void)
{
	std::string prefix, suffix;

	_numAudioDrivers = SDL_GetNumAudioDrivers();
	_currentAudioDriver = SDL_GetCurrentAudioDriver();

	_synthesis << "Audio drivers : " << _numAudioDrivers << std::endl;

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
		_synthesis
			<< "\t" << prefix << "'" <<
			SDL_GetAudioDriver(driverIndex)
			<< "'" << suffix << std::endl;
	}

}

void Introspection::fetchRenderDriverInfo(SDL_RendererInfo const & rendererInfo)
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

	_synthesis
		<< "\t'" << rendererInfo.name << "' [" << flags << "] "
		<< "[" << rendererInfo.max_texture_width << "x"
		<< rendererInfo.max_texture_height << "] "
		<< "has " << rendererInfo.num_texture_formats << " pixel formats"
		<< std::endl;

	for (Uint32 formatIndex(0);
		formatIndex < rendererInfo.num_texture_formats;
		++formatIndex)
		_synthesis << "\t\t" << SDL_GetPixelFormatName(
			rendererInfo.texture_formats[formatIndex]) << std::endl;
}

void Introspection::fetchRenderDrivers(void)
{
	_numRenderDrivers = SDL_GetNumRenderDrivers();

	_synthesis << "Render drivers : " << _numRenderDrivers << std::endl;
	for(int driverIndex(0) ;
		driverIndex < _numRenderDrivers ;
		++driverIndex)
	{
		SDL_RendererInfo rendererInfo;
		SDL_GetRenderDriverInfo(driverIndex, &rendererInfo);
		fetchRenderDriverInfo(rendererInfo);
	}

}

void Introspection::perform(void)
{
	fetchVideoDisplays();
	fetchAudioDevices();
	fetchVideoDrivers();
	fetchAudioDrivers();
	fetchRenderDrivers();

	INFO(SDL_LOG_CATEGORY_APPLICATION, "\n%s", _synthesis.str().c_str());
}

void Introspection::insertRendererInfo(SDL_Renderer * renderer)
{
	SDL_RendererInfo rendererInfo;
	SDL_GetRendererInfo(renderer, &rendererInfo);
	fetchRenderDriverInfo(rendererInfo);
}