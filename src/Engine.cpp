#include <VBN/Engine.hpp>
#include <VBN/HandlerResponse.hpp>
#include <VBN/IGameContext.hpp>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_timer.h>
#include <numeric>

#define MS_PER_FRAME_STACK_SIZE 40

Engine::Engine(std::shared_ptr<IGameContext> initialContext) :
	_msPerFrame(1, 1000)
{
	_stack.push_back(initialContext);
}

void Engine::run(void)
{
	SDL_Event ev;

	Sint32 remaining(0);

	Uint32 startTime(0),
		duration(0),
		nominalDuration(1000/60),
		gameTicks(0);

	float ratio(0.01);

	std::shared_ptr<HandlerResponse> response(new HandlerResponse);
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
			"Nominal frame duration : %d ms",
			nominalDuration);

	/* One loop equals one frame */
	while(!_stack.empty())
	{
/* ---- Begin chrono measure */
		startTime = SDL_GetTicks();

		/* Input */
		while (SDL_PollEvent(&ev) != 0)
			_stack.back()->handleEvent(ev, response);

		/* Output */
		_stack.back()->display();

		/* Time */
		_stack.back()->elapse(nominalDuration * ratio);

		duration = SDL_GetTicks() - startTime;
/* ----- End chrono measure */

/* ----- Begin chrono correction */
		remaining = nominalDuration - duration;
		if (remaining > 0)
			SDL_Delay(remaining);
		else if (remaining < 0)
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
				"Frame was too long to prepare : %d ms",
				duration);
			_stack.back()->elapse((duration - nominalDuration) * ratio);
		}
/* ----- End chrono correction */

		_msPerFrame.push_front(SDL_GetTicks() - startTime);
		while(_msPerFrame.size() > MS_PER_FRAME_STACK_SIZE)
			_msPerFrame.pop_back();

		if (response->getPopFlag())
		{
			_stack.pop_back();
			response->resetPopFlag();
		}
		else if (response->getNextIGameContext())
		{
			_stack.push_back(response->getNextIGameContext());
			response->resetNextIGameContext();
		}
	}
}

Uint32 Engine::getAverageMillisecondsPerFrame(void)
{
	/* Compute the sum of all elapsed milliseconds during last frames */
	Uint32 sum = std::accumulate(_msPerFrame.begin(),
				_msPerFrame.end(),
				0);

	/* Divide by size (=average) and return */
	return sum / _msPerFrame.size();
}

Uint32 Engine::getInstantMillisecondsPerFrame(void)
{
	return _msPerFrame.front();
}
