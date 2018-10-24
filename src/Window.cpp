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

	setBlendMode(SDL_BLENDMODE_BLEND);

	/* Apply ratioType-specific settings */
	applyRatioTypeSettings();

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

void Window::applyRatioTypeSettings(void)
{
	Uint32 windowFlags = SDL_GetWindowFlags(_window.get());

	/* Windowed */
	if ((windowFlags & SDL_WINDOW_FULLSCREEN_DESKTOP) == false)
	{
		switch (_ratioType)
		{
			case FIXED_RATIO_STRETCH:
				SDL_RenderSetLogicalSize(
					_renderer.get(),
					_canvasWidth,
					_canvasHeight);
			break;
			case FIXED_RATIO_FRAME:
				SDL_RenderSetLogicalSize(_renderer.get(), 0, 0);

				SDL_Rect windowSurface;
				SDL_GetWindowSize(
					_window.get(),
					&windowSurface.w,
					&windowSurface.h);
				updateCanvasFrame(windowSurface);
			break;
			case DYNAMIC_RATIO:
				SDL_RenderSetLogicalSize(_renderer.get(), 0, 0);
			break;
		}
	}
	/* Fullscreen */
	else
	{
		switch (_ratioType)
		{
			case FIXED_RATIO_STRETCH:
				SDL_RenderSetLogicalSize(
					_renderer.get(),
					_canvasWidth,
					_canvasHeight);
			break;
			case FIXED_RATIO_FRAME:
				SDL_RenderSetLogicalSize(_renderer.get(), 0, 0);

				SDL_Rect desktopSurface;
				SDL_GetDisplayBounds(SDL_GetWindowDisplayIndex(_window.get()),
					&desktopSurface);
				updateCanvasFrame(desktopSurface);
			break;
			case DYNAMIC_RATIO:
				SDL_RenderSetLogicalSize(_renderer.get(), 0, 0);
			break;
		}
	}
}

void Window::updateCanvasFrame(SDL_Rect const & outerSurface)
{
	if (_ratioType == FIXED_RATIO_FRAME)
	{
		int hShift = (outerSurface.w - _canvasWidth) / 2;
		int vShift = (outerSurface.h - _canvasHeight) / 2;
		SDL_Rect viewport{ hShift, vShift, _canvasWidth, _canvasHeight };
		SDL_RenderSetViewport(_renderer.get(), &viewport);
	}
}

void Window::toggleFullscreen(void)
{
	Uint32 currentFlags = SDL_GetWindowFlags(_window.get());
	bool fullscreen = (currentFlags & SDL_WINDOW_FULLSCREEN_DESKTOP);

	if (fullscreen)
	{
		SDL_SetWindowFullscreen(_window.get(), 0);
		applyRatioTypeSettings();
	}
	else
	{
		SDL_SetWindowFullscreen(_window.get(), SDL_WINDOW_FULLSCREEN_DESKTOP);
		applyRatioTypeSettings();
	}
}

void Window::setBlendMode(SDL_BlendMode const & blendMode)
{
	if (SDL_SetRenderDrawBlendMode(_renderer.get(), blendMode))
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"SDL_SetRenderDrawBlendMode() returned '%s'", SDL_GetError());
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

void Window::SDLWindowDeleter::operator()(SDL_Window * window) const
{
	SDL_DestroyWindow(window);
}

void Window::SDLRendererDeleter::operator()(SDL_Renderer * renderer) const
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

void Window::handleResize(void)
{
	applyRatioTypeSettings();
}