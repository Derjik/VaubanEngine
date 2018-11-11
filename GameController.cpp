#include <VBN/GameController.hpp>
#include <VBN/Logging.hpp>
#include <VBN/Exceptions.hpp>

GameController::GameController(unsigned const deviceIndex) :
	_controller(nullptr, &SDL_GameControllerClose),
	_deviceIndex(deviceIndex),
	_instanceId(-1),
	_haptic(nullptr, &SDL_HapticClose),
	_rumbleSupport(false)
{
	INFO(SDL_LOG_CATEGORY_INPUT, "Opening device #%d", deviceIndex);

	_controller = std::unique_ptr<SDL_GameController, decltype(&SDL_GameControllerClose)>(
		SDL_GameControllerOpen(deviceIndex),
		SDL_GameControllerClose);
	if (!_controller)
		THROW(Exception,
			"Could not open device #%d : SDL error '%s'",
			deviceIndex, SDL_GetError());

	SDL_Joystick * joystick = SDL_GameControllerGetJoystick(_controller.get());
	if (!joystick)
		THROW(Exception,
			"Could not retrieve joystick from device #%d : SDL error '%s'",
			deviceIndex, SDL_GetError());

	_instanceId = SDL_JoystickInstanceID(joystick);
	if (_instanceId < 0)
		THROW(Exception,
			"Bad instance ID returned by SDL_JoystickInstanceID : "
			"SDL error '%s'",
			SDL_GetError());

	if (SDL_JoystickIsHaptic(joystick))
	{
		DEBUG(SDL_LOG_CATEGORY_INPUT,
			"Controller has haptic support, trying to open SDL_Haptic...");
		_haptic = std::unique_ptr<SDL_Haptic, decltype(&SDL_HapticClose)>(
			SDL_HapticOpenFromJoystick(joystick),
			&SDL_HapticClose);
		DEBUG(SDL_LOG_CATEGORY_INPUT,
			"Done opening SDL_Haptic");

		DEBUG(SDL_LOG_CATEGORY_INPUT,
			"SDL_Haptic supports %d effects",
			SDL_HapticNumEffects(_haptic.get()));

		if (SDL_HapticRumbleSupported(_haptic.get()) == SDL_TRUE)
		{
			int hapticInfo = SDL_HapticQuery(_haptic.get());
			if(hapticInfo == 0)
				ERROR(SDL_LOG_CATEGORY_ERROR,
					"Could not read haptic information for instance @%d : "
					"SDL error %s",
					_instanceId,
					SDL_GetError());
			else
			{
				if (hapticInfo & SDL_HAPTIC_CONSTANT)
					INFO(SDL_LOG_CATEGORY_INPUT,
						"Instance @%d : haptic supports CONSTANT",
						_instanceId);
				if(hapticInfo & SDL_HAPTIC_SINE)
					INFO(SDL_LOG_CATEGORY_INPUT,
						"Instance @%d : haptic supports SINE",
						_instanceId);
				if(hapticInfo & SDL_HAPTIC_LEFTRIGHT)
					INFO(SDL_LOG_CATEGORY_INPUT,
						"Instance @%d : haptic supports LEFTRIGHT",
						_instanceId);
				if(hapticInfo & SDL_HAPTIC_TRIANGLE)
					INFO(SDL_LOG_CATEGORY_INPUT,
						"Instance @%d : haptic supports TRIANGLE",
						_instanceId);
				if(hapticInfo & SDL_HAPTIC_SAWTOOTHUP)
					INFO(SDL_LOG_CATEGORY_INPUT,
						"Instance @%d : haptic supports SAWTOOTHUP",
						_instanceId);
				if(hapticInfo & SDL_HAPTIC_SAWTOOTHDOWN)
					INFO(SDL_LOG_CATEGORY_INPUT,
						"Instance @%d : haptic supports SAWTOOTHDOWN",
						_instanceId);
				if(hapticInfo & SDL_HAPTIC_RAMP)
					INFO(SDL_LOG_CATEGORY_INPUT,
						"Instance @%d : haptic supports RAMP",
						_instanceId);
				if(hapticInfo & SDL_HAPTIC_SPRING)
					INFO(SDL_LOG_CATEGORY_INPUT,
						"Instance @%d : haptic supports SPRING",
						_instanceId);
				if(hapticInfo & SDL_HAPTIC_DAMPER)
					INFO(SDL_LOG_CATEGORY_INPUT,
						"Instance @%d : haptic supports DAMPER",
						_instanceId);
				if(hapticInfo & SDL_HAPTIC_INERTIA)
					INFO(SDL_LOG_CATEGORY_INPUT,
						"Instance @%d : haptic supports INERTIA",
						_instanceId);
				if(hapticInfo & SDL_HAPTIC_FRICTION)
					INFO(SDL_LOG_CATEGORY_INPUT,
						"Instance @%d : haptic supports FRICTION",
						_instanceId);
				if(hapticInfo & SDL_HAPTIC_CUSTOM)
					INFO(SDL_LOG_CATEGORY_INPUT,
						"Instance @%d : haptic supports CUSTOM",
						_instanceId);
				if(hapticInfo & SDL_HAPTIC_GAIN)
					INFO(SDL_LOG_CATEGORY_INPUT,
						"Instance @%d : haptic supports GAIN",
						_instanceId);
				if(hapticInfo & SDL_HAPTIC_AUTOCENTER)
					INFO(SDL_LOG_CATEGORY_INPUT,
						"Instance @%d : haptic supports AUTOCENTER",
						_instanceId);
				if(hapticInfo & SDL_HAPTIC_STATUS)
					INFO(SDL_LOG_CATEGORY_INPUT,
						"Instance @%d : haptic supports STATUS",
						_instanceId);
			}

			_rumbleSupport = true;
			if (SDL_HapticRumbleInit(_haptic.get()) < 0)
				ERROR(SDL_LOG_CATEGORY_ERROR,
					"Error while initializing haptic rumble: SDL error '%s'",
					SDL_GetError());
		}
	}
}

int GameController::getDeviceIndex(void) const
{
	return _deviceIndex;
}

SDL_JoystickID GameController::getInstanceId(void) const
{
	return _instanceId;
}

SDL_GameController * GameController::getSDLGameController(void)
{
	return _controller.get();
}

SDL_Joystick * GameController::getSDLJoystick(void)
{
	return SDL_GameControllerGetJoystick(_controller.get());
}

SDL_Haptic * GameController::getSDLHaptic(void)
{
	return _haptic.get();
}

void GameController::rumble(float strength, Uint32 milliseconds)
{
	if(_haptic && _rumbleSupport)
		if (SDL_HapticRumblePlay(_haptic.get(), strength, milliseconds) < 0)
			ERROR(SDL_LOG_CATEGORY_APPLICATION,
				"Could not play rumble effect : SDL error '%s'",
				SDL_GetError());
}

GameController::GameController(GameController && other) :
	_controller(std::move(other._controller)),
	_deviceIndex(std::move(other._deviceIndex)),
	_instanceId(std::move(other._instanceId)),
	_haptic(std::move(other._haptic)),
	_rumbleSupport(std::move(other._rumbleSupport))
{}

GameController & GameController::operator = (GameController && other)
{
	this->_controller = std::move(other._controller);
	this->_deviceIndex = std::move(other._deviceIndex);
	this->_instanceId = std::move(other._instanceId);
	this->_haptic = std::move(other._haptic);
	this->_rumbleSupport = std::move(other._rumbleSupport);
	return (*this);
}
