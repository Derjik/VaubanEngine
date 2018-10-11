#include <VBN/GameContext.hpp>
#include <VBN/GameControllerManager.hpp>
#include <VBN/WindowManager.hpp>
#include <SDL2/SDL_log.h>

GameContext::GameContext(std::shared_ptr<WindowManager> wm) : _windowManager(wm)
{}

void GameContext::handleEvent(SDL_Event const & event)
{
	switch(event.type)
	{
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			handleKeyboardEvent(event);
		break;

		case SDL_CONTROLLERDEVICEADDED:
		case SDL_CONTROLLERDEVICEREMOVED:
		case SDL_CONTROLLERAXISMOTION:
		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERBUTTONUP:
			handleGameControllerEvent(event);
		break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			handleMouseEvent(event);
		break;

		case SDL_WINDOWEVENT:
			handleWindowEvent(event);
		break;
	}
}

void GameContext::handleGameControllerEvent(SDL_Event const & event)
{
	switch(event.type)
	{
		case SDL_CONTROLLERDEVICEADDED:
			SDL_LogInfo(SDL_LOG_CATEGORY_INPUT,
				"[EVENT] Device #%d added",
				event.cdevice.which);

			GameControllerManager::openFromDeviceIndex(event.cdevice.which);
		break;
		case SDL_CONTROLLERDEVICEREMOVED:
			SDL_LogInfo(SDL_LOG_CATEGORY_INPUT,
				"[EVENT] Instance @%d removed",
				event.cdevice.which);

			GameControllerManager::closeFromInstance(event.cdevice.which);
		break;
		case SDL_CONTROLLERBUTTONDOWN:
			SDL_LogInfo(SDL_LOG_CATEGORY_INPUT,
				"[EVENT] Instance @%d - Button %d pressed",
				event.cdevice.which,
				(int)(event.cbutton.button));
		break;
		case SDL_CONTROLLERAXISMOTION:
			SDL_LogInfo(SDL_LOG_CATEGORY_INPUT,
				"[EVENT] Joystick moved (%d)",
				event.jaxis.timestamp);
		break;
	}
}

void GameContext::handleKeyboardEvent(SDL_Event const & event)
{
	Uint32 keyEvType = event.key.type;
	Window & main(_windowManager->getByName("mainWindow"));

	switch(event.key.keysym.sym)
	{
		case SDLK_F11:
			if(keyEvType == SDL_KEYDOWN)
				main.toggleFullscreen();
		break;
	}
}

void GameContext::handleMouseEvent(SDL_Event const & event)
{}

void GameContext::handleWindowEvent(SDL_Event const & event)
{
	switch (event.window.event)
	{
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			SDL_Window * address = SDL_GetWindowFromID(event.window.windowID);
			std::string windowName = _windowManager->getWindowNameByAddress(address);

			Window & win = _windowManager->getByAddress(address);

			SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
				"Window '%s' (ID: %d) changed size to %dx%d",
				windowName.c_str(), win.getId(),
				event.window.data1, event.window.data2);

			win.handleResize();

		break;
	}
}