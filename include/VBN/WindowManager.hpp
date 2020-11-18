#ifndef WINDOW_MANAGER_HPP_INCLUDED
#define WINDOW_MANAGER_HPP_INCLUDED

#include <memory>
#include <map>
#include <VBN/Window.hpp>

class TrueTypeFontManager;

/*!
 * Stores & retrieves Window objects 
 *
 * The WindowManager is the main component handling Window creation and storage
 * application-wide
 */
class WindowManager
{
	private:
		//! Actual Window map using SDL-provided IDs for indexing
		std::map<Uint32, Window> _idToWindow;
		//! User-friendly string-indexed map to name each Window
		std::map<std::string, Uint32> _stringToId;

	public:
		//! Build a WindowManager
		WindowManager(void);
		//! Delete a WindowManager
		~WindowManager(void);

		WindowManager(WindowManager const &) = delete;
		WindowManager(WindowManager &&) = delete;
		WindowManager & operator = (WindowManager const &) = delete;
		WindowManager & operator = (WindowManager &&) = delete;

		//! Instantiate a new Window and store it into the internal map
		void addWindow(
			std::string const & name,
			std::string const & title,
			int x, int y,
			int width, int height,
			Window::RatioType ratioType,
			Uint32 windowFlags,
			Uint32 rendererFlags,
			std::shared_ptr<TrueTypeFontManager> ttfManager);

		//! Get Window by human-readable name
		Window * getWindowByName(std::string const &);
		//! Get Window by SDL_Window ID
		Window * getWindowById(Uint32 const id);
};

#endif // WINDOW_MANAGER_HPP_INCLUDED
