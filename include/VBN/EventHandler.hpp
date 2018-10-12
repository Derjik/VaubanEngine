#ifndef EVENT_HANDLER_HPP_INCLUDED
#define EVENT_HANDLER_HPP_INCLUDED

#include <SDL2/SDL_events.h>

class EventHandler
{
	public:
		virtual bool handleEvent(SDL_Event const &) = 0;
};

#endif // EVENT_HANDLER_HPP_INCLUDED
