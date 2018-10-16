#ifndef GAME_CONTROLLER_MANAGER_HPP_INCLUDED
#define GAME_CONTROLLER_MANAGER_HPP_INCLUDED

#include <map>

class GameControllerManager
{
	private:
		std::map<unsigned, unsigned> _instanceToDevice;
		std::map<unsigned, unsigned> _deviceToInstance;

	public:
		~GameControllerManager(void);
		bool openFromDeviceIndex(unsigned const);
		bool closeFromInstance(unsigned const);
};

#endif // GAME_CONTROLLER_MANAGER_HPP_INCLUDED
