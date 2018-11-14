#ifndef ENGINE_UPDATE_HPP_INCLUDED
#define ENGINE_UPDATE_HPP_INCLUDED

#include <memory>

class IGameContext;

class EngineUpdate
{
	private:
		bool _popFlag;
		std::shared_ptr<IGameContext> _nextIGameContext;

	public:
		EngineUpdate(void);
		EngineUpdate(EngineUpdate const &) = delete;
		EngineUpdate(EngineUpdate &&) = delete;
		EngineUpdate & operator = (EngineUpdate const &) = delete;
		EngineUpdate & operator = (EngineUpdate &&) = delete;
		~EngineUpdate(void);

		/* API for the EventHandler */
		void pushGameContext(std::shared_ptr<IGameContext>);
		void popGameContext(void);

		/* API for the Engine */
		std::shared_ptr<IGameContext> getNextIGameContext(void);
		void resetNextIGameContext(void);
		bool getPopFlag(void);
		void resetPopFlag(void);
};

#endif // ENGINE_UPDATE_HPP_INCLUDED
