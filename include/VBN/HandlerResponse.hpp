#ifndef HANDLER_RESPONSE_HPP_INCLUDED
#define HANDLER_RESPONSE_HPP_INCLUDED

#include <memory>

class IGameContext;

class HandlerResponse
{
	private:
		bool _popFlag;
		std::shared_ptr<IGameContext> _nextIGameContext;

	public:
		HandlerResponse(void);

		/* API for the EventHandler */
		void pushGameContext(std::shared_ptr<IGameContext>);
		void popGameContext(void);

		/* API for the Engine */
		std::shared_ptr<IGameContext> getNextIGameContext(void);
		void resetNextIGameContext(void);
		bool getPopFlag(void);
		void resetPopFlag(void);
};

#endif // HANDLER_RESPONSE_HPP_INCLUDED
