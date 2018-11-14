#include <VBN/Platform.hpp>
#include <VBN/Exceptions.hpp>

Platform::Platform(WindowManager * windowManager,
	GameControllerManager * gameControllerManager) :
	_windowManager(windowManager),
	_gameControllerManager(gameControllerManager)
{
	if (!windowManager)
		THROW(Exception, "Received nullptr 'windowManager'");
	if (!gameControllerManager)
		THROW(Exception, "Received nullptr 'gameControllerManager'");
}

WindowManager * Platform::getWindowManager(void)
{
	return _windowManager.get();
}

GameControllerManager * Platform::getGameControllerManager(void)
{
	return _gameControllerManager.get();
}