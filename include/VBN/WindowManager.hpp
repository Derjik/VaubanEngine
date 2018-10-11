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
		std::map<SDL_Window *, std::string> _labels;

	public:
		void add(
			std::string const & name,
			std::string const & title,
			int x, int y,
			int width, int height,
			RatioType ratioType,
			Uint32 windowFlags,
			Uint32 rendererFlags,
			std::shared_ptr<TrueTypeFontManager> & ttfManager);

		Window & getByName(std::string const &);
		Window & getByAddress(SDL_Window *);
		std::string getWindowNameByAddress(SDL_Window *);
		void remove(std::string const &);
};

#endif // WINDOW_MANAGER_HPP_INCLUDED
