#ifndef I_GAME_CONTEXT_INCLUDED_HPP
#define I_GAME_CONTEXT_INCLUDED_HPP

#include <SDL2/SDL_events.h>
#include <VBN/WindowManager.hpp>

class HandlerResponse;

class IGameContext
{
	public:
		/* This method intends to handle all polled SDL_Event structures,
		and fill the HandlerResponse if needed to update IGameContext stack */
		virtual void handleEvent(
			SDL_Event const & event,
			std::shared_ptr<HandlerResponse> response) = 0;

		/* This method must perform a complete drawing (including RenderPresent)
		of the adequate scene for the specialized context */
		virtual void display(void) = 0;

		/* This is the time computation method */
		virtual void elapse(Uint32 gameTicks) = 0;
};

#endif // I_GAME_CONTEXT_INCLUDED_HPP
