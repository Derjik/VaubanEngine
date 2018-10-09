#include <SDL2/SDL_log.h>
#include <VBN/GameControllerManager.hpp>

std::map<unsigned, unsigned>
GameControllerManager::_instanceToDevice;

std::map<unsigned, unsigned>
GameControllerManager::_deviceToInstance;

void GameControllerManager::clean(void)
{
	SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Cleaning controllers");

	/* Close all controllers */
	for(auto & gcPair : _deviceToInstance)
		SDL_GameControllerClose(SDL_GameControllerFromInstanceID(gcPair.second));

	/* Clear mappings */
	_deviceToInstance.clear();
	_instanceToDevice.clear();

	SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Cleaning controllers done");
}

bool GameControllerManager::openFromDeviceIndex(unsigned const index)
{
	SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Opening device #%d", index);

	SDL_GameController * newGC = SDL_GameControllerOpen(index);

	if(newGC == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_INPUT,
			"GameControllerManager::openFromDeviceIndex: "
			"Could not open device #%d : %s",
			index, SDL_GetError());

		return false;
	}
	else
	{
		SDL_Joystick * newJoystick = SDL_GameControllerGetJoystick(newGC);
		unsigned instanceId = SDL_JoystickInstanceID(newJoystick);

		_deviceToInstance.insert(std::make_pair(index, instanceId));
		_instanceToDevice.insert(std::make_pair(instanceId, index));
	}

	SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Current mappings :");
	SDL_LogDebug(SDL_LOG_CATEGORY_INPUT,
		"#Device\t@Instance\tName\t\t\tAddress");

	for(auto const & p : _deviceToInstance)
	{
		SDL_GameController * gc = SDL_GameControllerFromInstanceID(p.second);

		SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "%d\t%d\t\t%s\t%p",
				p.first,
				p.second,
				SDL_GameControllerName(gc),
				gc);
	}

	return true;
}

bool GameControllerManager::closeFromInstance(unsigned const instance)
{
	/* Look for the instance to close in current mappings */
	auto iterator = _instanceToDevice.find(instance);

	/* If absent, return false and print an error message */
	if(iterator == _instanceToDevice.end())
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			"GameControllerManager::closeFromInstance: "
			"Cannot close instance @%d : not found in mappings",
			instance);

		return false;
	}

	/* Retrieve the controller from its instance ID */
	SDL_GameController *
		gcToClose = SDL_GameControllerFromInstanceID(instance);

	/* Ensure that SDL did not return a NULL pointer */
	if(gcToClose == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			"GameControllerManager::closeFromInstance: "
			"Cannot close instance @%d : not returned by SDL",
			instance);

		return false;
	}

	SDL_LogDebug(SDL_LOG_CATEGORY_INPUT,
		"GameControllerManager::closeFromInstance: "
		"Closing instance @%d at address %p",
		instance,
		gcToClose);

	SDL_GameControllerClose(gcToClose);

	SDL_LogDebug(SDL_LOG_CATEGORY_INPUT,
		"GameControllerManager::closeFromInstance: "
		"Removing from mappings");

	_deviceToInstance.erase(iterator->second);
	_instanceToDevice.erase(iterator);

	return true;
}
