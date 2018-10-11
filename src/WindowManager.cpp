#include <VBN/WindowManager.hpp>
#include <VBN/Window.hpp>
#include <SDL2/SDL_log.h>

void WindowManager::add(
	std::string const & name,
	std::string const & title,
	int x, int y,
	int w, int h,
	RatioType ratioType,
	Uint32 windowFlags,
	Uint32 rendererFlags,
	std::shared_ptr<TrueTypeFontManager> & ttfManager)
{
	if(title.empty() || name.empty())
		return;

	_windows.emplace(make_pair(name, Window(title, x, y, w, h, windowFlags, rendererFlags, ttfManager)));
}

Window & WindowManager::get(std::string const & name)
{
	if(_windows.find(name) != _windows.end())
		return _windows.at(name);
	else
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			"No window named '%s'",
			name.c_str());
		throw(std::string("No window named " + name));
	}
}

void WindowManager::remove(std::string const & name)
{
	if(_windows.find(name) != _windows.end())
		_windows.erase(name);
}