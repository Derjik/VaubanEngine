#ifndef WINDOW_HPP_INCLUDED
#define WINDOW_HPP_INCLUDED

#include <memory>
#include <string>
#include <SDL2/SDL_video.h>
#include <VBN/Renderer.hpp>

class TrueTypeFontManager;

/*!
 * SDL_Window wrapper class
 *
 * Each instance of this class encapsulates an SDL_Window object in a dedicated
 * std::unique_ptr, configured to free the underlying memory chunk using
 * SDL_DestroyWindow().
 */
class Window
{
	public:
		//! Used to configure "window framing" strategy
		enum RatioType
		{
			/*!
			 * Creates a sub-frame with black borders when the window is
			 * bigger than intended resolution
			 */
			FIXED_RATIO_FRAME,

			/*!
			 * Stretches display to ensure intended resolution, using black
			 * borders to fill gaps
			 */
			FIXED_RATIO_STRETCH,

			/*!
			 * Resolution depends on window size
			 */
			DYNAMIC_RATIO
		};

	private:
		//! Underlying SDL_Window enclosed in std::unique_ptr
		std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> _window;
		//! Renderer associated with the Window
		std::unique_ptr<Renderer> _renderer;

		//! Current window framing strategy
		RatioType _ratioType;
		//! Width of the drawable internal canvas
		int _canvasWidth;
		//! Height of the drawable internal canvas
		int _canvasHeight;

		//! (Re-)Apply RatioType, e.g. after toggling Full-Screen
		void applyRatioTypeSettings(void);

		//! Update canvas dimensions (if using FIXED_RATIO_FRAME mode)
		void updateCanvasFrame(SDL_Rect const & outerSurface);

	public:
		//! Build a Window
		Window(std::string const & title,
			int xPosition, int yPosition,
			int windowWidth, int windowHeight,
			RatioType ratioType,
			Uint32 windowFlags,
			Uint32 rendererFlags,
			std::shared_ptr<TrueTypeFontManager> ttfManager);
		//! Move a Window instance
		Window(Window && other);
		//! Destroy a Window
		~Window(void);

		Window(Window const & other) = delete;
		Window & operator = (Window const &) = delete;
		Window & operator = (Window &&) = delete;

		//! Toggle Full-Screen mode for the Window
		void toggleFullscreen(void);

		//! Get current decoration borders size */
		std::array<int, 4> getBorderSize(void) const;
		//! Get current size on screen */
		std::pair<int, int> getSize(void) const;
		//! Get current brightness level */
		float getBrightness(void) const;
		//! Get current display (screen) index */
		int getCurrentDisplayIndex(void) const;

		//! Get raw-pointer to associated Renderer */
		Renderer * getRenderer(void);
		//! Get underlying SDL_Window ID */
		Uint32 getId(void);

		//! Must be called whenever the Window is resized */
		void handleResize(void);
};

#endif // WINDOW_HPP_INCLUDED
