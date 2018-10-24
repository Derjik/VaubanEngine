#include <VBN/WindowManager.hpp>
#include <VBN/Window.hpp>
#include <SDL2/SDL_log.h>

void WindowManager::add(
	std::string const & name,
	std::string const & title,
	int x, int y,
	int w, int h,
	Window::RatioType ratioType,
	Uint32 windowFlags,
	Uint32 rendererFlags,
	std::shared_ptr<TrueTypeFontManager> ttfManager)
{
	if(title.empty() || name.empty())
		return;

	_windows.emplace(std::make_pair(name,
		Window(title,
			x, y, w, h,
			ratioType,
			windowFlags,
			rendererFlags,
			ttfManager)));
	_labels.emplace(std::make_pair(_windows.at(name).getAddress(),
		name));
}

Window & WindowManager::getByName(std::string const & name)
{
	/* Look for {name, Window} */
	if (_windows.find(name) != _windows.end())
		return _windows.at(name);
	else
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "No window with name '%s'", name);
		throw(std::string("No window with name '" + name + "'"));
	}
}

Window & WindowManager::getByAddress(SDL_Window * address)
{
	/* Look for {id, name} */
	if (_labels.find(address) != _labels.end())
	{
		/* Look for {id, Window} */
		try
		{
			getByName(_labels.at(address));
		}
		/* Mapping corrupted (severe) */
		catch (std::string str)
		{
			SDL_LogError(SDL_LOG_CATEGORY_ERROR,
				"WindowManager mapping error : found '%p' in _labels "
				"but not in _windows", address);
			throw(std::string("WindowManager mapping error : found '" +
				std::to_string(unsigned(address)) +
				" in _labels but not in _windows"));
		}
	}
	/* No such window */
	else
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			"No window with address '%p'", address);
		throw(std::string("No window with address '" +
			std::to_string(unsigned(address)) + "'"));
	}
}

void WindowManager::remove(std::string const & name)
{
	if (_windows.find(name) != _windows.end())
	{
		SDL_Window * address = _windows.at(name).getAddress();
		_windows.erase(name);
		_labels.erase(address);
	}
}

std::string WindowManager::getWindowNameByAddress(SDL_Window * address)
{
	if (_labels.find(address) != _labels.end())
		return _labels.at(address);
	else
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			"No window with address '%p'", address);
		throw(std::string("No window with address '" +
			std::to_string(unsigned(address)) + "'"));
	}
}