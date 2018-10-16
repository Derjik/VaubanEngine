#include <VBN/HandlerResponse.hpp>

HandlerResponse::HandlerResponse(void) : 
	_popFlag(false),
	_nextIGameContext(nullptr)
{}

void HandlerResponse::pushGameContext(std::shared_ptr<IGameContext> IGameContext)
{
	if (!_nextIGameContext)
		_nextIGameContext = IGameContext;
}

void HandlerResponse::popGameContext(void)
{
	_popFlag = true;
}

std::shared_ptr<IGameContext> HandlerResponse::getNextIGameContext(void)
{
	return _nextIGameContext;
}

void HandlerResponse::resetNextIGameContext(void)
{
	_nextIGameContext.reset();
}

bool HandlerResponse::getPopFlag(void)
{
	return _popFlag;
}

void HandlerResponse::resetPopFlag(void)
{
	_popFlag = false;
}