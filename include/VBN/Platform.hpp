#ifndef PLATFORM_HPP_INCLUDED
#define PLATFORM_HPP_INCLUDED

#include <memory>
#include <VBN/WindowManager.hpp>
#include <VBN/GameControllerManager.hpp>
#include <VBN/Mixer.hpp>

class Platform
{
	private:
		std::unique_ptr<WindowManager> _windowManager;
		std::unique_ptr<GameControllerManager> _gameControllerManager;
		std::unique_ptr<Mixer> _mixer;

	public:
		// May throw
		Platform(
			WindowManager * windowManager,
			GameControllerManager * gameControllerManager,
			Mixer * mixer);
		Platform(Platform const &) = delete;
		Platform & operator = (Platform const &) = delete;
		~Platform(void);

		WindowManager * getWindowManager(void);
		GameControllerManager * getGameControllerManager(void);
		Mixer * getMixer(void);
};

#endif // PLATFORM_HPP_INCLUDED
