#include <VBN/Window.hpp>
#include <VBN/Logging.hpp>
#include <VBN/Exceptions.hpp>

Window::Window(std::string const & title,
	int xPosition, int yPosition,
	int windowWidth, int windowHeight,
	RatioType ratioType,
	Uint32 windowFlags,
	Uint32 rendererFlags,
	std::shared_ptr<TrueTypeFontManager> ttfManager) :
	_window(nullptr, &SDL_DestroyWindow),
	_ratioType(ratioType),
	_canvasWidth(windowWidth),
	_canvasHeight(windowHeight),
	_renderer(nullptr),
	_bitmapFontManager(nullptr)
{
	if (title.empty())
		THROW(Exception, "Received empty 'title'");
	if (xPosition <= 0)
		THROW(Exception, "Received 'x' <= 0");
	if (yPosition <= 0)
		THROW(Exception, "Received 'y' <= 0");
	if (windowWidth <= 0)
		THROW(Exception, "Received 'w' <= 0");
	if (windowHeight <= 0)
		THROW(Exception, "Received 'h' <= 0");
	if (!ttfManager)
		THROW(Exception, "Received nullptr 'ttfManager'");

	_window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>(
		SDL_CreateWindow(title.c_str(),
						xPosition, yPosition,
						windowWidth, windowHeight,
						windowFlags),
		SDL_DestroyWindow);
	if(_window == nullptr)
		THROW(Exception,
			"Cannot instantiate SDL_Window : SDL error '%s'",
			SDL_GetError());

	_renderer = std::unique_ptr<Renderer>(new Renderer(
			_window.get(),
			SDL_RENDERER_ACCELERATED,
			ttfManager));

	applyRatioTypeSettings();

	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Build Window %p (SDL_Window %p)",
		this,
		_window.get());
}

Window::Window(Window && other) :
	_window(std::move(other._window)),
	_renderer(std::move(other._renderer)),
	_ratioType(std::move(other._ratioType)),
	_canvasWidth(std::move(other._canvasWidth)),
	_canvasHeight(std::move(other._canvasHeight)),
	_bitmapFontManager(std::move(other._bitmapFontManager)),
	_textures(std::move(other._textures))
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Move Window %p (SDL_Window %p) into new Window %p",
		&other,
		_window.get(),
		this);
}

Window::~Window(void)
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Delete Window %p (SDL_Window %p)",
		this,
		_window.get());
}

void Window::applyRatioTypeSettings(void)
{
	Uint32 windowFlags = SDL_GetWindowFlags(_window.get());

	/* Currently windowed */
	if ((windowFlags & SDL_WINDOW_FULLSCREEN_DESKTOP) == false)
	{
		switch (_ratioType)
		{
			case FIXED_RATIO_STRETCH:
				_renderer->setLogicalSize(_canvasWidth, _canvasHeight);
			break;
			case FIXED_RATIO_FRAME:
				_renderer->setLogicalSize(0, 0);

				SDL_Rect windowSurface;
				SDL_GetWindowSize(
					_window.get(),
					&windowSurface.w,
					&windowSurface.h);

				updateCanvasFrame(windowSurface);
			break;
			case DYNAMIC_RATIO:
				_renderer->setLogicalSize(0, 0);
			break;
		}
	}
	/* Currently fullscreen */
	else
	{
		switch (_ratioType)
		{
			case FIXED_RATIO_STRETCH:
				_renderer->setLogicalSize(_canvasWidth, _canvasHeight);
			break;
			case FIXED_RATIO_FRAME:
				_renderer->setLogicalSize(0, 0);

				SDL_Rect desktopSurface;
				SDL_GetDisplayBounds(SDL_GetWindowDisplayIndex(_window.get()),
					&desktopSurface);

				updateCanvasFrame(desktopSurface);
			break;
			case DYNAMIC_RATIO:
				_renderer->setLogicalSize(0, 0);
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
		_renderer->setViewport(viewport);
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

std::pair<int, int> Window::getSize(void) const
{
	std::pair<int, int> windowSize;
	SDL_GetWindowSize(_window.get(), &windowSize.first, &windowSize.second);
	return windowSize;
}

float Window::getBrightness(void) const
{
	return SDL_GetWindowBrightness(_window.get());
}

int Window::getCurrentDisplayIndex(void) const
{
	return SDL_GetWindowDisplayIndex(_window.get());
}

Renderer * Window::getRenderer(void)
{
	return _renderer.get();
}

Uint32 Window::getId(void)
{
	return SDL_GetWindowID(_window.get());
}

void Window::handleResize(void)
{
	applyRatioTypeSettings();
}