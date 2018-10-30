#include <VBN/EngineUpdate.hpp>
#include <VBN/Exceptions.hpp>

EngineUpdate::EngineUpdate(void) : 
	_popFlag(false),
	_nextIGameContext(nullptr)
{}

void EngineUpdate::pushGameContext(std::shared_ptr<IGameContext> gameContext)
{
	if (!gameContext)
		THROW(Exception, "Received nullptr 'gameContext'");

	if (!_nextIGameContext)
		_nextIGameContext = gameContext;
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