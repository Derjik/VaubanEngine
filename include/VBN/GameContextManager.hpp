#ifndef GAME_CONTEXT_MANAGER_HPP_INCLUDED
#define GAME_CONTEXT_MANAGER_HPP_INCLUDED

#include <memory>
#include <vector>
#include <deque>
#include <VBN/GameContext.hpp>

class GameContextManager
{
	private:
		static std::vector<std::shared_ptr<GameContext>> _stack;
		static std::deque<Uint32> _msPerFrame;
		static bool _pop;

	public:
		/* Run the main event loop */
		static void run(void);

		static Uint32 getAverageMillisecondsPerFrame(void);
		static Uint32 getInstantMillisecondsPerFrame(void);

		/* Push a GameContext to the top of the stack */
		static void push(std::shared_ptr<GameContext> context);
		/* Instruct the manager to pop the top GameContext off the stack next
		 * time fluh() is called */
		static void pop(void);
		/* Checks the _pop flag and pops the top GameContext if true */
		static void flush(void);
};

#endif // GAME_CONTEXT_MANAGER_HPP_INCLUDED
