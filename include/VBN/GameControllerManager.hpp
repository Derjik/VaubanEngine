#ifndef GAME_CONTROLLER_MANAGER_HPP_INCLUDED
#define GAME_CONTROLLER_MANAGER_HPP_INCLUDED

#include <map>
#include <SDL2/SDL_haptic.h>
#include <SDL2/SDL_gamecontroller.h>

class GameControllerManager
{
	private:
		std::map<unsigned, unsigned> _deviceToInstance;
		std::map<unsigned, unsigned> _instanceToDevice;
		std::map<unsigned, SDL_Haptic*> _instanceToHaptic;

	public:
		~GameControllerManager(void);
		void openFromDeviceIndex(unsigned const);
		void closeInstance(unsigned const);
		void logMappings(void);

		SDL_Haptic * getHapticFromInstance(unsigned const);
		SDL_GameController * getFirstController(void);
};

#endif // GAME_CONTROLLER_MANAGER_HPP_INCLUDED
