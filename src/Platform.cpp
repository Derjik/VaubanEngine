#include <VBN/Platform.hpp>
#include <VBN/Logging.hpp>
#include <VBN/Exceptions.hpp>

Platform::Platform(WindowManager * windowManager,
	GameControllerManager * gameControllerManager,
	Mixer * mixer) :
	_windowManager(windowManager),
	_gameControllerManager(gameControllerManager),
	_mixer(mixer)
{
	if (!windowManager)
		THROW(Exception, "Received nullptr 'windowManager'");
	if (!gameControllerManager)
		THROW(Exception, "Received nullptr 'gameControllerManager'");
	if (!mixer)
		THROW(Exception, "Received nullptr 'mixer'");

	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Build Platform %p (WindowManager %p ; GameControllerManager %p)",
		this,
		_windowManager.get(),
		_gameControllerManager.get());
}

Platform::~Platform(void)
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Delete Platform %p (WindowManager %p ; GameControllerManager %p)",
		this,
		_windowManager.get(),
		_gameControllerManager.get());
}

WindowManager * Platform::getWindowManager(void)
{
	return _windowManager.get();
}

GameControllerManager * Platform::getGameControllerManager(void)
{
	return _gameControllerManager.get();
}

Mixer * Platform::getMixer(void)
{
	return _mixer.get();
}