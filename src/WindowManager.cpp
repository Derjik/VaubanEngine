#include <VBN/WindowManager.hpp>
#include <VBN/Window.hpp>
#include <VBN/Exceptions.hpp>
#include <VBN/Logging.hpp>

WindowManager::WindowManager(void)
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Build WindowManager %p",
		this);
}

WindowManager::~WindowManager(void)
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Delete WindowManager %p",
		this);
}

/*!
 * This method tries instantiating a new Window object using given parameters,
 * and stores the resulting Window in the internal WindowManager::_idToWindow
 * map if the Window construction was successful.
 *
 * @param	title			UTF-8-encoded title
 * @param	xPosition		Initial X coordinate for the top-left corner
 * @param	yPosition		Initial Y coordinate for the top-left corner
 * @param	windowWidth		Window width
 * @param	windowHeight	Window height
 * @param	ratioType		See Window::RatioType
 * @param	windowFlags		Configuration flags passed to SDL_CreateWindow()
 * @param	rendererFlags	Configuration flags passed to Renderer()
 * @param	ttfManager		TrueTypeFontManager to use for text rendering
 * @throws	Exception		Invalid input parameters or SDL call error
 */
void WindowManager::addWindow(
	std::string const & name,
	std::string const & title,
	int x, int y,
	int w, int h,
	Window::RatioType ratioType,
	Uint32 windowFlags,
	Uint32 rendererFlags,
	std::shared_ptr<TrueTypeFontManager> ttfManager)
{
	// Attempt Window instantiation (may throw)
	Window win(
		title,
		x, y, w, h,
		ratioType,
		windowFlags,
		rendererFlags,
		ttfManager);

	// Acquire SDL_Window ID
	Uint32 id(win.getId());

	// Store name-ID association
	_stringToId.emplace(name, id);
	// Store ID-Window association
	_idToWindow.emplace(id, std::move(win));
}

/*!
 * Get a raw pointer to a stored Window using its human-readable name
 *
 * @param	name	std::string handle for the Window
 * @returns			A raw pointer to the associated Window if it exists, nullptr
 *					otherwise
 */
Window * WindowManager::getWindowByName(std::string const & name)
{
	if (_stringToId.find(name) != _stringToId.end())
		if (_idToWindow.find(_stringToId.at(name)) != _idToWindow.end())
			return (&_idToWindow.at(_stringToId.at(name)));
		else
			ERROR(SDL_LOG_CATEGORY_APPLICATION,
				"Window '%s' has id '%d' but cannot be found in _stringToId",
				name,
				_stringToId.at(name));

	return nullptr;
}

/*!
 * Get a raw pointer to a stored Window using its SDL_Window ID
 *
 * @param	id	SDL_Window ID of the Window
 * @returns		A raw pointer to the associated Window if it exists, nullptr
 *				otherwise
 */
Window * WindowManager::getWindowById(Uint32 const id)
{
	if (_idToWindow.find(id) != _idToWindow.end())
		return (&_idToWindow.at(id));

	return nullptr;
}
