#include <VBN/GameContextManager.hpp>
#include <numeric>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_timer.h>

#define MS_PER_FRAME_STACK_SIZE 40

std::vector<std::shared_ptr<GameContext>> GameContextManager::_stack;
std::deque<Uint32> GameContextManager::_msPerFrame(1, 1000);
bool GameContextManager::_pop(false);

void GameContextManager::run(void)
{
	SDL_Event ev;

	Uint32 startTime(0), duration(0), nominalDuration(1000/60);
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
			"Nominal frame duration : %d ms",
			nominalDuration);

	/* One loop equals one frame */
	while(!_stack.empty())
	{
/* ---- Begin chrono measure */
		startTime = SDL_GetTicks();

		/* Make the current GameContext handle all queued events */
		while(SDL_PollEvent(&ev) != 0)
			_stack.back()->handleEvent(ev);

		/* Make the current GameContext draw itself on screen */
		_stack.back()->draw();

		duration = SDL_GetTicks() - startTime;
/* ----- End chrono measure */

/* ----- Begin chrono correction */

		/* Frame was to short */
		if(duration < nominalDuration)
			/* Wait for a delta time */
			SDL_Delay(nominalDuration - duration);

		/* Frame was to long */
		else if (duration > nominalDuration)
		{
			/* Warn in logs */
			/* TODO : gameticks correction goes here */
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
				"Frame was too long to prepare : %d ms",
				duration);
		}
/* ----- End chrono correction */

/* ----- Begin chrono statistics */
		_msPerFrame.push_front(SDL_GetTicks() - startTime);
		while(_msPerFrame.size() > MS_PER_FRAME_STACK_SIZE)
			_msPerFrame.pop_back();
/* ----- End chrono statistics */

		/* If the _pop flag is set, pop the current GameContext off the stack */
		flush();
	}
}

Uint32 GameContextManager::getAverageMillisecondsPerFrame(void)
{
	/* Compute the sum of all elapsed milliseconds during last frames */
	Uint32 sum = std::accumulate(_msPerFrame.begin(),
				_msPerFrame.end(),
				0);

	/* Divide by size (=average) and return */
	return sum / _msPerFrame.size();
}

Uint32 GameContextManager::getInstantMillisecondsPerFrame(void)
{
	return _msPerFrame.front();
}

void GameContextManager::pop(void)
{
	_pop = true;
}

void GameContextManager::flush(void)
{
	if(_pop)
	{
		_pop = false;
		_stack.pop_back();
	}
}

void GameContextManager::push(std::shared_ptr<GameContext> context)
{
	_stack.push_back(context);
}