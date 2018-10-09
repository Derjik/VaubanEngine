#ifndef WINDOW_HPP_INCLUDED
#define WINDOW_HPP_INCLUDED

#include <memory>
#include <string>
#include <map>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <VBN/BitmapFontManager.hpp>
#include <VBN/Texture.hpp>

/* SDL_Window raw pointer deleter */
struct SDLWindowDeleter { void operator () (SDL_Window * window) const; };
/* SDL_Renderer raw pointer deleter */
struct SDLRendererDeleter { void operator () (SDL_Renderer * renderer) const; };

// Forward declarations
class TrueTypeFontManager;

class Window
{
	private:
		/* Encapsulated SDL_Window object */
		std::unique_ptr<SDL_Window, SDLWindowDeleter> _window;

		/* Encapsulated SDL_Renderer object */
		std::unique_ptr<SDL_Renderer, SDLRendererDeleter> _renderer;

		/* Encapsulated BitmapFontManager object */
		std::unique_ptr<BitmapFontManager> _bitmapFontManager;

		/* Texture storage map */
		std::map<std::string, Texture> _textures;

	public:
		/* Default constructor */
		Window(std::shared_ptr<TrueTypeFontManager> ttfManager);

		/*** Intermediary constructors ***/
		Window(std::string const & title,
			std::shared_ptr<TrueTypeFontManager> ttfManager);
		Window(std::string const & title,
			int x, int y,
			int width, int height,
			std::shared_ptr<TrueTypeFontManager> ttfManager);
		Window(std::string const & title,
			int x, int y,
			int width, int height,
			Uint32 windowFlags,
			std::shared_ptr<TrueTypeFontManager> ttfManager);

		/* Full constructor */
		Window(std::string const & title,
			int x, int y,
			int width, int height,
			Uint32 windowFlags,
			Uint32 rendererFlags,
			std::shared_ptr<TrueTypeFontManager> ttfManager);

		/* Copy constructor call is forbidden */
		Window(Window const & other) = delete;

		/* Move constructor */
		Window(Window && other);

		/* Set/unset fullscreen */
		void toggleFullscreen(void);

		/* Instantiate and insert a Texture with the specified text */
		bool addTextTexture(
				std::string const & textureName,
				std::shared_ptr<TrueTypeFontManager> ttfManager,
				std::string const & fontName,
				std::string const & text,
				int const size,
				SDL_Color const & color);

		bool removeTexture(std::string const & name);

		/* Print text at given location using a BitmapFont */
		void printText(std::string const & text,
				std::string const & fontName,
				int const size,
				SDL_Color const & color,
				SDL_Rect const & destination);

//		void printDebugText(std::string const & fontName,
//			int const size,
//			int const xDest,
//			int const yDest);

		Texture & getTexture(std::string const & name);

		std::array<int, 4> getBorderSize(void) const;
		float getBrightness(void) const;
		int getCurrentDisplayIndex(void) const;
		SDL_Renderer * getRenderer(void);

};

#endif // WINDOW_HPP_INCLUDED
