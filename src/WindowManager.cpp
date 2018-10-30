#include <VBN/WindowManager.hpp>
#include <VBN/Window.hpp>
#include <VBN/Exceptions.hpp>
#include <VBN/Logging.hpp>

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
	if (name.empty())
		THROW(Exception, "Received empty 'name'");
	if (title.empty())
		THROW(Exception, "Received empty 'title'");
	if (x <= 0)
		THROW(Exception, "Received 'x' <= 0");
	if (y <= 0)
		THROW(Exception, "Received 'y' <= 0");
	if (w <= 0)
		THROW(Exception, "Received 'w' <= 0");
	if (h <= 0)
		THROW(Exception, "Received 'h' <= 0");
	if (!ttfManager)
		THROW(Exception, "Received nullptr 'ttfManager'");

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
	if (_windows.find(name) != _windows.end())
		return _windows.at(name);
	else
		THROW(Exception, "No window with name '%s'", name);
}

Window & WindowManager::getByAddress(SDL_Window * address)
{
	if (_labels.find(address) != _labels.end())
	{
		try
		{
			getByName(_labels.at(address));
		}
		catch (Exception exc)
		{
			THROW(Exception,
				"Mapping error : found '%p' in _labels "
				"but not in _windows", address);
		}
	}
	else
		THROW(Exception, "No window with address '%p'", address);
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
		ERROR(SDL_LOG_CATEGORY_ERROR, "No window with address '%p'", address);
}