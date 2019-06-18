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
	if (name.empty())
		THROW(Exception, "Received empty 'name'");

	Window win(
		title,
		x, y, w, h,
		ratioType,
		windowFlags,
		rendererFlags,
		ttfManager);
	Uint32 id(win.getId());

	_stringToId.emplace(name, id);
	_idToWindow.emplace(id, std::move(win));
}

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

Window * WindowManager::getWindowById(Uint32 const id)
{
	if (_idToWindow.find(id) != _idToWindow.end())
		return (&_idToWindow.at(id));

	return nullptr;
}
