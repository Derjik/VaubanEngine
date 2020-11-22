#ifndef PLATFORM_HPP_INCLUDED
#define PLATFORM_HPP_INCLUDED

#include <memory>
#include <VBN/WindowManager.hpp>
#include <VBN/GameControllerManager.hpp>
#include <VBN/Mixer.hpp>

/*!
 * Aggregates global platform elements : WindowManager, GameControllerManager,
 * Mixer
 *
 * This class holds global handles to the main components of the current 
 * running platform
 */
class Platform
{
	private:
		//! Stores & manages Window instances
		std::unique_ptr<WindowManager> _windowManager;
		//! Stores & manages GameController instances
		std::unique_ptr<GameControllerManager> _gameControllerManager;
		//! Handles sound effects & music playback
		std::unique_ptr<Mixer> _mixer;

	public:
		//! Build a Platform
		Platform(
			WindowManager * windowManager,
			GameControllerManager * gameControllerManager,
			Mixer * mixer);
		//! Delete a Platform
		~Platform(void);

		Platform(Platform const &) = delete;
		Platform & operator = (Platform const &) = delete;

		//! Get a raw pointer to the WindowManager
		WindowManager * getWindowManager(void);
		//! Get a raw pointer to the GameControllerManager
		GameControllerManager * getGameControllerManager(void);
		//! Get a raw pointer to the Mixer
		Mixer * getMixer(void);
};

#endif // PLATFORM_HPP_INCLUDED
