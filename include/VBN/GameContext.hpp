#ifndef GAME_CONTEXT_INCLUDED_HPP
#define GAME_CONTEXT_INCLUDED_HPP

#include <SDL2/SDL_events.h>
#include <VBN/WindowManager.hpp>

class GameContext
{
	protected:
		std::shared_ptr<WindowManager> _windowManager;

		virtual void handleKeyboardEvent(
				SDL_Event const & event);
		virtual void handleMouseEvent(
				SDL_Event const & event);
		virtual void handleGameControllerEvent(
				SDL_Event const & event);
		virtual void handleWindowEvent(
				SDL_Event const & event);

	public:
		GameContext(std::shared_ptr<WindowManager> wm);

		void handleEvent(SDL_Event const & event);
		virtual void draw(void) = 0;
};

#endif // GAME_CONTEXT_INCLUDED_HPP
