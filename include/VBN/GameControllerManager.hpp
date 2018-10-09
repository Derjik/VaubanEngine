#ifndef GAME_CONTROLLER_MANAGER_HPP_INCLUDED
#define GAME_CONTROLLER_MANAGER_HPP_INCLUDED

#include <map>
#include <SDL2/SDL_gamecontroller.h>

class GameControllerManager
{
	private:
		static std::map<unsigned, unsigned> _instanceToDevice;
		static std::map<unsigned, unsigned> _deviceToInstance;

	public:
		static void clean(void);
		static bool openFromDeviceIndex(unsigned const);
		static bool closeFromInstance(unsigned const);
};

#endif // GAME_CONTROLLER_MANAGER_HPP_INCLUDED
