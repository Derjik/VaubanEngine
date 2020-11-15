#ifndef WINDOW_MANAGER_HPP_INCLUDED
#define WINDOW_MANAGER_HPP_INCLUDED

#include <memory>
#include <map>
#include <VBN/Window.hpp>

class TrueTypeFontManager;

/*!
 * Stores & retrieves Window objects 
 */
class WindowManager
{
	private:
		/* Actual Window map using SDL-provided IDs for indexing */
		std::map<Uint32, Window> _idToWindow;
		/* User-friendly string-indexed map to name each Window */
		std::map<std::string, Uint32> _stringToId;

	public:
		/* Constructors & destructor */
		WindowManager(void);
		WindowManager(WindowManager const &) = delete;
		WindowManager(WindowManager &&) = delete;
		WindowManager & operator = (WindowManager const &) = delete;
		WindowManager & operator = (WindowManager &&) = delete;
		~WindowManager(void);

		/* Create a Window and add store it into the Manager */
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

		/* Getters */
		// By human-readable name
		Window * getWindowByName(std::string const &);
		// By SDL_Window ID
		Window * getWindowById(Uint32 const id);
};

#endif // WINDOW_MANAGER_HPP_INCLUDED
