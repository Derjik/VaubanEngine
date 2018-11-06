#include <VBN/Platform.hpp>
#include <VBN/Exceptions.hpp>

Platform::Platform(std::shared_ptr<WindowManager> windowManager,
	std::shared_ptr<GameControllerManager> gameControllerManager) :
	_windowManager(windowManager),
	_gameControllerManager(gameControllerManager)
{
	if (!windowManager)
		THROW(Exception, "Received nullptr 'windowManager'");
	if (!gameControllerManager)
		THROW(Exception, "Received nullptr 'gameControllerManager'");
}

std::shared_ptr<WindowManager> Platform::getWindowManager(void)
{
	return _windowManager;
}

std::shared_ptr<GameControllerManager> Platform::getGameControllerManager(void)
{
	return _gameControllerManager;
}