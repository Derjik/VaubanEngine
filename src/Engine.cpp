#include <VBN/Engine.hpp>
#include <VBN/EngineUpdate.hpp>
#include <VBN/IGameContext.hpp>
#include <SDL2/SDL_timer.h>
#include <VBN/Logging.hpp>
#include <numeric>

#define MS_PER_FRAME_STACK_SIZE 40

Engine::Engine(std::shared_ptr<IGameContext> initialContext) :
	_msPerFrame(1, 1000)
{
	_stack.push_back(initialContext);
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Build Engine %p",
		this);
}

Engine::~Engine(void)
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Delete Engine %p",
		this);
}

void Engine::run(float const gameTicksPerMillisecond)
{
	SDL_Event ev;

	Uint32  frameStartTime(0),
			frameDuration(0),
			nominalFrameDuration(1000 / 60);

	Sint32 unusedTime(0);

	std::shared_ptr<EngineUpdate> update(new EngineUpdate);

	INFO(SDL_LOG_CATEGORY_APPLICATION,
			"Nominal frame frameDuration : %d ms",
			nominalFrameDuration);

	/* One loop equals one frame */
	while(!_stack.empty())
	{
/* ---- Begin chrono measure */
		frameStartTime = SDL_GetTicks();

		/* Input */
		while (SDL_PollEvent(&ev) != 0)
			_stack.back()->handleEvent(ev, update);

		/* Time */
		_stack.back()->elapse(
			(Uint32)((float)(nominalFrameDuration) * gameTicksPerMillisecond),
			update);

		/* Output */
		_stack.back()->display();

		frameDuration = SDL_GetTicks() - frameStartTime;
/* ----- End chrono measure */

/* ----- Begin chrono correction */
		unusedTime = nominalFrameDuration - frameDuration;
		if (unusedTime > 0)
			SDL_Delay(unusedTime);
		else if (unusedTime < 0)
		{
			WARNING(SDL_LOG_CATEGORY_APPLICATION,
				"Frame was too long to prepare : %d ms",
				frameDuration);
			_stack.back()->elapse(
				(Uint32)((float)(-unusedTime) * gameTicksPerMillisecond),
				update);
		}
/* ----- End chrono correction */

		_msPerFrame.push_front(SDL_GetTicks() - frameStartTime);
		while(_msPerFrame.size() > MS_PER_FRAME_STACK_SIZE)
			_msPerFrame.pop_back();

		if (update->getPopFlag())
		{
			_stack.pop_back();
			update->resetPopFlag();
		}
		else if (update->getNextIGameContext())
		{
			_stack.push_back(update->getNextIGameContext());
			update->resetNextIGameContext();
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
