#include <VBN/GameControllerManager.hpp>
#include <VBN/Logging.hpp>
#include <VBN/Exceptions.hpp>
#include <string>

GameControllerManager::GameControllerManager(void)
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Build GameControllerManager %p",
		this);
}

GameControllerManager::~GameControllerManager(void)
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Delete GameControllerManager %p",
		this);
}

void GameControllerManager::openFromDeviceIndex(int const deviceIndex)
{
	GameController controller(deviceIndex);
	SDL_JoystickID instanceId = controller.getInstanceId();

	auto newPair = _controllers.emplace(
		deviceIndex,
		std::move(controller));

	_deviceToInstance.insert(std::make_pair(deviceIndex, instanceId));
	_instanceToDevice.insert(std::make_pair(instanceId, deviceIndex));

	logMappings();
}

void GameControllerManager::logMappings(void)
{
	INFO(SDL_LOG_CATEGORY_INPUT, "Current mappings :");
	INFO(SDL_LOG_CATEGORY_INPUT, "#Device\t@Instance\tAddress\t\t\tName");

	for(auto & p : _controllers)
	{
		INFO(SDL_LOG_CATEGORY_INPUT,
			"%d\t%d\t%p\t%s",
			p.second.getDeviceIndex(),
			p.second.getInstanceId(),
			p.second.getSDLGameController(),
			SDL_GameControllerName(p.second.getSDLGameController()));
	}
}

void GameControllerManager::closeInstance(SDL_JoystickID const instanceId)
{
	INFO(SDL_LOG_CATEGORY_INPUT, "Closing instance @%d", instanceId);
	auto mappingIterator = _instanceToDevice.find(instanceId);
	if (mappingIterator != _instanceToDevice.end())
	{
		auto deviceIterator = _instanceToDevice.find(instanceId);
		if (deviceIterator == _instanceToDevice.end())
			ERROR(SDL_LOG_CATEGORY_ERROR,
				"No instance @%d in _instanceToDevice map",
				instanceId);

		SDL_JoystickID deviceId = deviceIterator->second;

		if (_controllers.find(instanceId) == _controllers.end())
			ERROR(SDL_LOG_CATEGORY_ERROR,
				"No instance @%d in _controllers map",
				instanceId);

		_controllers.erase(_instanceToDevice.at(instanceId));

		if (_deviceToInstance.find(deviceId) == _deviceToInstance.end())
			ERROR(SDL_LOG_CATEGORY_ERROR,
				"No device #%d found in _deviceToInstance map",
				deviceId);

		_deviceToInstance.erase(deviceId);
		_instanceToDevice.erase(instanceId);
	}
}

int GameControllerManager::getNumAvailableControllers(void) const
{
	return _controllers.size();
}

GameController * GameControllerManager::getControllerFromDeviceID(int deviceID)
{
	if (_controllers.find(deviceID) == _controllers.end())
		return nullptr;
	else
		return (&_controllers.at(deviceID));
}

GameController * GameControllerManager::getControllerFromInstanceID(SDL_JoystickID instanceID)
{
	auto deviceIterator(_instanceToDevice.find(instanceID));
	if(deviceIterator == _instanceToDevice.end())
		return nullptr;

	auto controllerIterator = _controllers.find(deviceIterator->second);
	if (controllerIterator == _controllers.end())
	{
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Bad mappings : instanceID %d mapped to device %d without "
			"associated controller",
			instanceID, deviceIterator->second);
		return nullptr;
	}

	return (&controllerIterator->second);
}
