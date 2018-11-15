#ifndef GAME_CONTROLLER_HPP_INCLUDED
#define GAME_CONTROLLER_HPP_INCLUDED

#include <memory>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_haptic.h>

class GameController
{
	private:
		std::unique_ptr<SDL_GameController, decltype(&SDL_GameControllerClose)> _controller;
		int _deviceIndex;
		SDL_JoystickID _instanceId;

		std::unique_ptr<SDL_Haptic, decltype(&SDL_HapticClose)> _haptic;
		bool _rumbleSupport;

	public:
		// May throw
		GameController(unsigned const deviceIndex);
		GameController(GameController const & other) = delete;
		GameController(GameController && other);
		GameController & operator = (GameController const &) = delete;
		GameController & operator = (GameController &&) = delete;
		~GameController(void);

		int getDeviceIndex(void) const;
		SDL_JoystickID getInstanceId(void) const;

		SDL_GameController * getSDLGameController(void);
		SDL_Joystick * getSDLJoystick(void);
		SDL_Haptic * getSDLHaptic(void);
		void rumble(float strength, Uint32 milliseconds);
};

#endif // GAME_CONTROLLER_HPP_INCLUDED
