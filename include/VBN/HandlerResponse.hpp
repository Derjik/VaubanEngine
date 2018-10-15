#ifndef HANDLER_RESPONSE_HPP_INCLUDED
#define HANDLER_RESPONSE_HPP_INCLUDED

#include <memory>

class GameContext;

class HandlerResponse
{
	private:
		bool _popFlag;
		std::shared_ptr<GameContext> _nextGameContext;

	public:
		HandlerResponse(void);

		/* API for the EventHandler */
		void pushGameContext(std::shared_ptr<GameContext>);
		void popGameContext(void);

		/* API for the Engine */
		std::shared_ptr<GameContext> getNextGameContext(void);
		void resetNextGameContext(void);
		bool getPopFlag(void);
		void resetPopFlag(void);
};

#endif // HANDLER_RESPONSE_HPP_INCLUDED
