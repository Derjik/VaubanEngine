#ifndef GAME_CONTEXT_INCLUDED_HPP
#define GAME_CONTEXT_INCLUDED_HPP

#include <SDL2/SDL_events.h>
#include <VBN/WindowManager.hpp>

class HandlerResponse;

class GameContext
{
	protected:
		std::shared_ptr<WindowManager> _windowManager;

		virtual void handleKeyboardEvent(
				SDL_Event const & event,
				std::shared_ptr<HandlerResponse> response);
		virtual void handleMouseEvent(
				SDL_Event const & event,
				std::shared_ptr<HandlerResponse> response);
		virtual void handleGameControllerEvent(
				SDL_Event const & event,
				std::shared_ptr<HandlerResponse> response);
		virtual void handleWindowEvent(
				SDL_Event const & event,
				std::shared_ptr<HandlerResponse> response);

	public:
		GameContext(std::shared_ptr<WindowManager> wm);

		void handleEvent(SDL_Event const & event,
			std::shared_ptr<HandlerResponse> response);
		virtual void draw(void) = 0;
};

#endif // GAME_CONTEXT_INCLUDED_HPP
