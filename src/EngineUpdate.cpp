#include <VBN/EngineUpdate.hpp>

EngineUpdate::EngineUpdate(void) : 
	_popFlag(false),
	_nextIGameContext(nullptr)
{}

void EngineUpdate::pushGameContext(std::shared_ptr<IGameContext> IGameContext)
{
	if (!_nextIGameContext)
		_nextIGameContext = IGameContext;
}

void EngineUpdate::popGameContext(void)
{
	_popFlag = true;
}

std::shared_ptr<IGameContext> EngineUpdate::getNextIGameContext(void)
{
	return _nextIGameContext;
}

void EngineUpdate::resetNextIGameContext(void)
{
	_nextIGameContext.reset();
}

bool EngineUpdate::getPopFlag(void)
{
	return _popFlag;
}

void EngineUpdate::resetPopFlag(void)
{
	_popFlag = false;
}