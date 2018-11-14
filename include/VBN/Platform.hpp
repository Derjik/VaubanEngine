#ifndef PLATFORM_HPP_INCLUDED
#define PLATFORM_HPP_INCLUDED

#include <memory>
#include <VBN/WindowManager.hpp>
#include <VBN/GameControllerManager.hpp>

class Platform
{
	private:
		std::unique_ptr<WindowManager> _windowManager;
		std::unique_ptr<GameControllerManager> _gameControllerManager;

	public:
		Platform(WindowManager * windowManager,
			GameControllerManager * gameControllerManager);
		Platform(Platform const &) = delete;
		Platform & operator = (Platform const &) = delete;
		~Platform(void);

		WindowManager * getWindowManager(void);
		GameControllerManager * getGameControllerManager(void);
};

#endif // PLATFORM_HPP_INCLUDED
