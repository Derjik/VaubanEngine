#ifndef PLATFORM_HPP_INCLUDED
#define PLATFORM_HPP_INCLUDED

#include <memory>

class WindowManager;
class GameControllerManager;

class Platform
{
	private:
		std::shared_ptr<WindowManager> _windowManager;
		std::shared_ptr<GameControllerManager> _gameControllerManager;

	public:
		Platform(std::shared_ptr<WindowManager>,
			std::shared_ptr<GameControllerManager>);

		std::shared_ptr<WindowManager> getWindowManager(void);
		std::shared_ptr<GameControllerManager> getGameControllerManager(void);
};

#endif // PLATFORM_HPP_INCLUDED
