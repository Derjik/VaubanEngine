#ifndef WINDOW_MANAGER_HPP_INCLUDED
#define WINDOW_MANAGER_HPP_INCLUDED

#include <memory>
#include <map>
#include <VBN/Window.hpp>

class TrueTypeFontManager;

class WindowManager
{
	private:
		std::map<Uint32, Window> _idToWindow;
		std::map<std::string, Uint32> _stringToId;

	public:
		WindowManager(void);
		WindowManager(WindowManager const &) = delete;
		WindowManager(WindowManager &&) = delete;
		WindowManager & operator = (WindowManager const &) = delete;
		WindowManager & operator = (WindowManager &&) = delete;
		~WindowManager(void);

		// May throw
		void addWindow(
			std::string const & name,
			std::string const & title,
			int x, int y,
			int width, int height,
			Window::RatioType ratioType,
			Uint32 windowFlags,
			Uint32 rendererFlags,
			std::shared_ptr<TrueTypeFontManager> ttfManager);

		Window * getWindowByName(std::string const &);
		Window * getWindowById(Uint32 const id);
};

#endif // WINDOW_MANAGER_HPP_INCLUDED
