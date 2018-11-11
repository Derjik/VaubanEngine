#ifndef GAME_CONTROLLER_MANAGER_HPP_INCLUDED
#define GAME_CONTROLLER_MANAGER_HPP_INCLUDED

#include <map>
#include <VBN/GameController.hpp>

class GameControllerManager
{
	private:
		std::map<int, SDL_JoystickID> _deviceToInstance;
		std::map<SDL_JoystickID, int> _instanceToDevice;
		std::map<int, GameController> _controllers;

		void logMappings(void);

	public:
		~GameControllerManager(void);

		void openFromDeviceIndex(int const);
		void closeInstance(SDL_JoystickID const);

		int getNumAvailableControllers(void) const;
		GameController * getControllerFromDeviceID(int deviceID);
		GameController * getControllerFromInstanceID(SDL_JoystickID instanceID);
};

#endif // GAME_CONTROLLER_MANAGER_HPP_INCLUDED
