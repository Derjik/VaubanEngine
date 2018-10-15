#include <VBN/HandlerResponse.hpp>

HandlerResponse::HandlerResponse(void) : 
	_popFlag(false),
	_nextGameContext(nullptr)
{}

void HandlerResponse::pushGameContext(std::shared_ptr<GameContext> gameContext)
{
	if (!_nextGameContext)
		_nextGameContext = gameContext;
}

void HandlerResponse::popGameContext(void)
{
	_popFlag = true;
}

std::shared_ptr<GameContext> HandlerResponse::getNextGameContext(void)
{
	return _nextGameContext;
}

void HandlerResponse::resetNextGameContext(void)
{
	_nextGameContext.reset();
}

bool HandlerResponse::getPopFlag(void)
{
	return _popFlag;
}

void HandlerResponse::resetPopFlag(void)
{
	_popFlag = false;
}