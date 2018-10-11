#include <VBN/Engine.hpp>
#include <VBN/Introspection.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


Engine::Engine(void) :
	_gameContextManager(nullptr),
	_windowManager(nullptr),
	_gameControllerManager(nullptr),
	_trueTypeFontManager(nullptr)
{
	/* Initialize SDL library and additional modules */
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	/* Set DEBUG mode for all logs */
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);

	/* Perform introspection */
	Introspection::perform();

	_windowManager = std::make_shared<WindowManager>();

	/* Initialize TTF fonts */
	_trueTypeFontManager = std::make_shared<TrueTypeFontManager>(std::set<std::string>({"courier", "arial"}));

	/* Instantiate main window */
	_windowManager->add("mainWindow", "SDL Main Window",
						SDL_WINDOWPOS_CENTERED,
						SDL_WINDOWPOS_CENTERED,
						1600, 900,
						RatioType::FIXED_RATIO_FRAME,
						SDL_WINDOW_SHOWN,
						SDL_RENDERER_ACCELERATED,
						_trueTypeFontManager);

	/* Insert main menu into context stack */
	//GameContextManager::push(std::shared_ptr<GameContext>(new GCMenu()));
	/* Run game */
	//GameContextManager::run();

	/* Close all modules */
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

Engine::~Engine(void)
{
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

std::shared_ptr<TrueTypeFontManager> Engine::getTrueTypeFontManager(void)
{
	return _trueTypeFontManager;
}