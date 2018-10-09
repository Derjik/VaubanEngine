#ifndef WINDOW_MANAGER_HPP_INCLUDED
#define WINDOW_MANAGER_HPP_INCLUDED

#include <memory>
#include <map>
#include <VBN/Window.hpp>

class TrueTypeFontManager;

class WindowManager
{
	private:
		std::map<std::string, Window> _windows;

	public:
		void add(
			std::string const & name,
			std::string const & title,
			int x, int y,
			int width, int height,
			Uint32 windowFlags,
			Uint32 rendererFlags,
			std::shared_ptr<TrueTypeFontManager> & ttfManager);

		Window & get(std::string const &);
		void remove(std::string const &);
};

#endif // WINDOW_MANAGER_HPP_INCLUDED
