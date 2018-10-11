#include <VBN/Window.hpp>
#include <VBN/Introspection.hpp>
#include <VBN/BitmapFontManager.hpp>
#include <SDL2/SDL_log.h>

/*
 * Full constructor
 */
Window::Window(std::string const & title,
	int xPosition, int yPosition,
	int windowWidth, int windowHeight,
	RatioType ratioType,
	Uint32 windowFlags,
	Uint32 rendererFlags,
	std::shared_ptr<TrueTypeFontManager> ttfManager) :
	_window(nullptr),
	_ratioType(ratioType),
	_canvasWidth(windowWidth),
	_canvasHeight(windowHeight),
	_renderer(nullptr),
	_bitmapFontManager(nullptr)
{
	/* Instantiate SDL Window */
	_window = std::unique_ptr<SDL_Window, SDLWindowDeleter>(
		SDL_CreateWindow(title.c_str(),
						xPosition, yPosition,
						windowWidth, windowHeight,
						windowFlags),
		SDLWindowDeleter());
	if(_window == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			"Window : SDL_CreateWindow() returned error '%s'",
			SDL_GetError());

		throw std::string("Window: SDL_CreateWindow() returned error '"
			+ std::string(SDL_GetError()) + "'");
	}

	/* Instantiate SDL Rendering Context */
	_renderer = std::unique_ptr<SDL_Renderer, SDLRendererDeleter>(
		SDL_CreateRenderer(_window.get(), -1, rendererFlags),
		SDLRendererDeleter());
	if(_renderer == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			"Window: SDL_CreateRenderer() returned error '%s'",
			SDL_GetError());
		throw std::string(
			"Window: SDL_CreateRenderer() returned error '"
			+ std::string(SDL_GetError()) + "'");
	}

	/* Introspect newly created context */
	SDL_RendererInfo rendererInfo;
	SDL_GetRendererInfo(_renderer.get(), &rendererInfo);
	Introspection::logRendererInfo(_renderer.get());

	/* Instantiate BitmapFontManager */
	_bitmapFontManager = std::unique_ptr<BitmapFontManager>(
		new BitmapFontManager(ttfManager, _renderer.get()));
	if(_bitmapFontManager == nullptr)
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			"Window: Failed to instantiate BitmapFontManager");
}

Window::Window(Window && other) :
	_window(std::move(other._window)),
	_ratioType(std::move(other._ratioType)),
	_canvasWidth(std::move(other._canvasWidth)),
	_canvasHeight(std::move(other._canvasHeight)),
	_renderer(std::move(other._renderer)),
	_textures(std::move(other._textures)),
	_bitmapFontManager(std::move(other._bitmapFontManager))
{}


void Window::toggleFullscreen(void)
{
	Uint32 currentFlags = SDL_GetWindowFlags(_window.get());

	/* Window currently on fullscreen */
	if((currentFlags & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
		SDL_SetWindowFullscreen(_window.get(), 0);
	else /* Window not on fullscreen */
		SDL_SetWindowFullscreen(_window.get(), SDL_WINDOW_FULLSCREEN_DESKTOP);
}

bool Window::addTextTexture(std::string const & textureName,
			std::shared_ptr<TrueTypeFontManager> ttfManager,
			std::string const & fontName,
			std::string const & text,
			int const size,
			SDL_Color const & color)
{
	/* Ensure uniqueness of textures */
	if(_textures.find(textureName) != _textures.end())
		return false;

	/* Add to mappings */
	_textures.emplace(
		make_pair(textureName,
			Texture::fromText(ttfManager, _renderer.get(),
				text, fontName, size, color)));

	return true;
}

Texture & Window::getTexture(std::string const & name)
{
	if(_textures.find(name) == _textures.end())
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			"Window::getTexture: No texture named '%s'",
			name.c_str());
		throw std::string("No texture named '" + name + "'");

	}
	else
		return _textures.at(name);
}

bool Window::removeTexture(std::string const & name)
{
	/* Ensure texture exists */
	auto iterator = _textures.find(name);
	if(iterator == _textures.end())
	{
		SDL_LogError(SDL_LOG_CATEGORY_RENDER,
			"Cannot remove texture '%s' : not found in mappings",
			name.c_str());
		return false;
	}

	_textures.erase(name);

	return true;
}

void Window::printText(std::string const & text,
		std::string const & fontName,
		int const size,
		SDL_Color const & color,
		SDL_Rect const & destination)
{
	BitmapFont & font = _bitmapFontManager->getFont(fontName, size);
	font.renderText(text, color, destination, _renderer.get());
}

std::array<int, 4> Window::getBorderSize(void) const
{
	std::array<int, 4> borderSize;
	SDL_GetWindowBordersSize(
		_window.get(),
		&borderSize.at(0),
		&borderSize.at(1),
		&borderSize.at(2),
		&borderSize.at(3));

	return borderSize;
}

float Window::getBrightness(void) const
{
	return SDL_GetWindowBrightness(_window.get());
}

int Window::getCurrentDisplayIndex(void) const
{
	return SDL_GetWindowDisplayIndex(_window.get());
}

SDL_Renderer * Window::getRenderer(void)
{
	return _renderer.get();
}

//void Window::printDebugText(std::string const & fontName,
//	int const size,
//	int const xDest,
//	int const yDest)
//{
//	BitmapFont & font = _bitmapFontManager->getFont(fontName, size);
//	font.renderDebug(_renderer.get(), xDest, yDest);
//}

void SDLWindowDeleter::operator()(SDL_Window * window) const
{
	SDL_DestroyWindow(window);
}

void SDLRendererDeleter::operator()(SDL_Renderer * renderer) const
{
	SDL_DestroyRenderer(renderer);
}

Uint32 Window::getId(void)
{
	return SDL_GetWindowID(_window.get());
}

SDL_Window * Window::getAddress(void)
{
	return _window.get();
}