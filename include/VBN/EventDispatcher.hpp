#ifndef EVENT_DISPATCHER_HPP_INCLUDED
#define EVENT_DISPATCHER_HPP_INCLUDED

#include <VBN/IEventHandler.hpp>
#include <memory>

class EventDispatcher : public IEventHandler
{
	private:
		std::shared_ptr<IEventHandler> _mouseEventHandler;
		std::shared_ptr<IEventHandler> _keyboardEventHandler;
		std::shared_ptr<IEventHandler> _gameControllerEventHandler;
		std::shared_ptr<IEventHandler> _joystickEventHandler;
		std::shared_ptr<IEventHandler> _windowEventHandler;

	public:
		EventDispatcher(
			std::shared_ptr<IEventHandler> mouse,
			std::shared_ptr<IEventHandler> keyboard,
			std::shared_ptr<IEventHandler> gameController,
			std::shared_ptr<IEventHandler> joystick,
			std::shared_ptr<IEventHandler> window);
		EventDispatcher(EventDispatcher const &) = delete;
		EventDispatcher(EventDispatcher &&) = delete;
		EventDispatcher & operator = (EventDispatcher const &) = delete;
		EventDispatcher & operator = (EventDispatcher &&) = delete;
		~EventDispatcher(void);

		void handleEvent(SDL_Event const & event,
			std::shared_ptr<EngineUpdate> engineUpdate);
};

#endif // EVENT_DISPATCHER_HPP_INCLUDED
