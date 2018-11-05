#include <VBN/EventDispatcher.hpp>
#include <VBN/Logging.hpp>

EventDispatcher::EventDispatcher(
	std::shared_ptr<IEventHandler> mouse,
	std::shared_ptr<IEventHandler> keyboard,
	std::shared_ptr<IEventHandler> gameController,
	std::shared_ptr<IEventHandler> joystick,
	std::shared_ptr<IEventHandler> window) :
	_mouseEventHandler(mouse),
	_keyboardEventHandler(keyboard),
	_gameControllerEventHandler(gameController),
	_joystickEventHandler(joystick),
	_windowEventHandler(window)
{}

EventDispatcher::~EventDispatcher(void)
{}

void EventDispatcher::handleEvent(SDL_Event const & event,
	std::shared_ptr<EngineUpdate> engineUpdate)
{
	switch(event.type)
	{
		case SDL_WINDOWEVENT:
		case SDL_SYSWMEVENT:
			if (_windowEventHandler)
				_windowEventHandler->handleEvent(event, engineUpdate);
		break;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
		case SDL_TEXTEDITING:
		case SDL_TEXTINPUT:
		case SDL_KEYMAPCHANGED:
			if (_keyboardEventHandler)
				_keyboardEventHandler->handleEvent(event, engineUpdate);
		break;

		case SDL_MOUSEMOTION:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEWHEEL:
			if (_mouseEventHandler)
				_mouseEventHandler->handleEvent(event, engineUpdate);
		break;

		case SDL_JOYAXISMOTION:
		case SDL_JOYBALLMOTION:
		case SDL_JOYHATMOTION:
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
		case SDL_JOYDEVICEADDED:
		case SDL_JOYDEVICEREMOVED:
			if (_joystickEventHandler)
				_joystickEventHandler->handleEvent(event, engineUpdate);
		break;

		case SDL_CONTROLLERAXISMOTION:
		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERBUTTONUP:
		case SDL_CONTROLLERDEVICEADDED:
		case SDL_CONTROLLERDEVICEREMOVED:
		case SDL_CONTROLLERDEVICEREMAPPED:
			if (_gameControllerEventHandler)
				_gameControllerEventHandler->handleEvent(event, engineUpdate);
		break;
	}
}