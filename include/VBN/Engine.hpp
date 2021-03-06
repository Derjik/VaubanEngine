#ifndef GAME_CONTEXT_MANAGER_HPP_INCLUDED
#define GAME_CONTEXT_MANAGER_HPP_INCLUDED

#include <SDL2/SDL_types.h>
#include <memory>
#include <vector>
#include <deque>

class IGameContext;

class Engine
{
	private:
		std::vector<std::shared_ptr<IGameContext>> _stack;
		std::deque<Uint32> _msPerFrame;

	public:
		Engine(std::shared_ptr<IGameContext> initialContext);
		Engine(Engine const &) = delete;
		Engine(Engine &&) = delete;
		Engine & operator = (Engine const &) = delete;
		Engine & operator = (Engine &&) = delete;
		~Engine(void);

		void run(float const gameTicksPerMillisecond);

		Uint32 getAverageMillisecondsPerFrame(void);
		Uint32 getInstantMillisecondsPerFrame(void);
};

#endif // GAME_CONTEXT_MANAGER_HPP_INCLUDED
