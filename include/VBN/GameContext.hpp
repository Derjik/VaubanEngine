#ifndef GAME_CONTEXT_INCLUDED_HPP
#define GAME_CONTEXT_INCLUDED_HPP

#include <SDL2/SDL_events.h>
#include <VBN/WindowManager.hpp>

class HandlerResponse;

class GameContext
{
	public:
		virtual void handleEvent(
			SDL_Event const & event,
			std::shared_ptr<HandlerResponse> response) = 0;

		virtual void draw(void) = 0;
};

#endif // GAME_CONTEXT_INCLUDED_HPP
