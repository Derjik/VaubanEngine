#ifndef ENGINE_HPP_INCLUDED
#define ENGINE_HPP_INCLUDED

#include <memory>
#include <VBN/GameContextManager.hpp>
#include <VBN/WindowManager.hpp>
#include <VBN/GameControllerManager.hpp>
#include <VBN/TrueTypeFontManager.hpp>

class Engine
{
	private:
		std::shared_ptr<GameContextManager> _gameContextManager;
		std::shared_ptr<WindowManager> _windowManager;
		std::shared_ptr<GameControllerManager> _gameControllerManager;
		std::shared_ptr<TrueTypeFontManager> _trueTypeFontManager;

	public:
		Engine(void);
		~Engine(void);

		std::shared_ptr<TrueTypeFontManager> getTrueTypeFontManager(void);
};

#endif // ENGINE_HPP_INCLUDED
