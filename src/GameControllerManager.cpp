#include <VBN/GameControllerManager.hpp>
#include <VBN/Logging.hpp>
#include <string>

GameControllerManager::~GameControllerManager(void)
{
	VERBOSE(SDL_LOG_CATEGORY_INPUT, "Cleaning controllers");

	for(auto & gcPair : _deviceToInstance)
		SDL_GameControllerClose(SDL_GameControllerFromInstanceID(gcPair.second));

	_deviceToInstance.clear();
	_instanceToDevice.clear();

	VERBOSE(SDL_LOG_CATEGORY_INPUT, "Done cleaning controllers");
}

void GameControllerManager::openFromDeviceIndex(unsigned const index)
{
	INFO(SDL_LOG_CATEGORY_INPUT, "Opening device #%d", index);

	SDL_GameController * newGC = SDL_GameControllerOpen(index);
	if (newGC == nullptr)
	{
		ERROR(SDL_LOG_CATEGORY_INPUT,
			"Could not open device #%d : SDL error '%s'",
			index, SDL_GetError());
		return;
	}

	VERBOSE(SDL_LOG_CATEGORY_INPUT,
		"Done opening controller, retrieving joystick");

	SDL_Joystick * newJoystick = SDL_GameControllerGetJoystick(newGC);
	if (newJoystick == nullptr)
	{
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Could not retrieve joystick from device #%d : SDL error '%s'",
			index, SDL_GetError());
		return;
	}

	VERBOSE(SDL_LOG_CATEGORY_INPUT,
		"Done retrieving joystick, retrieving instance ID");

	unsigned instanceId = SDL_JoystickInstanceID(newJoystick);
	if (instanceId < 0)
	{
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Bad instance ID returned by SDL_JoystickInstanceID : "
			"SDL error '%s'",
			SDL_GetError());
		return;
	}

	VERBOSE(SDL_LOG_CATEGORY_INPUT,
		"Done retrieving instance ID, looking for haptics");

	if (SDL_JoystickIsHaptic(newJoystick))
	{
		SDL_Haptic * newHaptic = SDL_HapticOpenFromJoystick(newJoystick);
		_instanceToHaptic.insert(std::make_pair(instanceId, newHaptic));

		if (SDL_HapticRumbleSupported(newHaptic) == SDL_TRUE)
			if (SDL_HapticRumbleInit(newHaptic) < 0)
				ERROR(SDL_LOG_CATEGORY_ERROR,
					"Error while initializing haptic: SDL error '%s'",
					SDL_GetError());
	}

	VERBOSE(SDL_LOG_CATEGORY_INPUT,
		"Done looking for haptics, adding to mappings");

	_deviceToInstance.insert(std::make_pair(index, instanceId));
	_instanceToDevice.insert(std::make_pair(instanceId, index));

	VERBOSE(SDL_LOG_CATEGORY_INPUT,
		"Done adding to mappings, printing mappings");

	logMappings();
}

void GameControllerManager::logMappings(void)
{
	INFO(SDL_LOG_CATEGORY_INPUT, "Current mappings :");
	INFO(SDL_LOG_CATEGORY_INPUT, "#Device\t@Instance\tName\t\t\tAddress");

	for(auto const & p : _deviceToInstance)
	{
		SDL_GameController * gc = SDL_GameControllerFromInstanceID(p.second);

		INFO(SDL_LOG_CATEGORY_INPUT, "%d\t%d\t\t%s\t%p",
				p.first,
				p.second,
				SDL_GameControllerName(gc),
				gc);
	}
}

void GameControllerManager::closeInstance(unsigned const instance)
{
	INFO(SDL_LOG_CATEGORY_INPUT, "Closing instance @%d", instance);

	auto deviceIterator = _instanceToDevice.find(instance);
	if (deviceIterator == _instanceToDevice.end())
	{
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Cannot close instance @%d : not found in mappings",
			instance);
		return;
	}

	SDL_GameController * gcToClose = SDL_GameControllerFromInstanceID(instance);
	if (gcToClose == nullptr)
	{
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Cannot close instance @%d : SDL error '%s'",
			instance, SDL_GetError());
		return;
	}

	auto hapticIterator = _instanceToHaptic.find(instance);
	if (hapticIterator != _instanceToHaptic.end())
	{
		SDL_HapticClose(hapticIterator->second);
		_instanceToHaptic.erase(instance);
	}

	VERBOSE(SDL_LOG_CATEGORY_INPUT,
		"GameControllerManager::closeFromInstance: "
		"Closing instance @%d at address %p",
		instance,
		gcToClose);

	SDL_GameControllerClose(gcToClose);

	VERBOSE(SDL_LOG_CATEGORY_INPUT,
		"GameControllerManager::closeFromInstance: "
		"Removing from mappings");

	_deviceToInstance.erase(deviceIterator->second);
	_instanceToDevice.erase(deviceIterator);
}

SDL_Haptic * GameControllerManager::getHapticFromInstance(
	unsigned const instance)
{
	if (_instanceToHaptic.find(instance) == _instanceToHaptic.end())
		return nullptr;
	else
		return _instanceToHaptic.at(instance);
}

SDL_GameController * GameControllerManager::getFirstController(void)
{
	if (!_instanceToDevice.empty())
		return SDL_GameControllerFromInstanceID(_instanceToDevice.begin()->first);
	else
		return nullptr;
}
