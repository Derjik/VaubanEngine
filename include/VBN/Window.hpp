#ifndef WINDOW_HPP_INCLUDED
#define WINDOW_HPP_INCLUDED

#include <memory>
#include <string>
#include <map>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <VBN/Texture.hpp>
#include <VBN/BitmapFontManager.hpp>

class TrueTypeFontManager;

class Window
{
	public:
		enum RatioType
		{
			FIXED_RATIO_FRAME,
			FIXED_RATIO_STRETCH,
			DYNAMIC_RATIO
		};

	private:
		struct SDLWindowDeleter { void operator () (SDL_Window * window) const; };
		std::unique_ptr<SDL_Window, SDLWindowDeleter> _window;

		struct SDLRendererDeleter { void operator () (SDL_Renderer * renderer) const; };
		std::unique_ptr<SDL_Renderer, SDLRendererDeleter> _renderer;

		RatioType _ratioType;
		unsigned _canvasWidth;
		unsigned _canvasHeight;

		std::unique_ptr<BitmapFontManager> _bitmapFontManager;

		std::map<std::string, Texture> _textures;

		void applyRatioTypeSettings(void);
		void updateCanvasFrame(SDL_Rect const & outerSurface);

	public:
		Window(std::string const & title,
			int xPosition, int yPosition,
			int windowWidth, int windowHeight,
			RatioType ratioType,
			Uint32 windowFlags,
			Uint32 rendererFlags,
			std::shared_ptr<TrueTypeFontManager> ttfManager);

		Window(Window const & other) = delete;
		Window(Window && other);

		void toggleFullscreen(void);
		void setBlendMode(SDL_BlendMode const & blendMode);

		void addTextTexture(
			std::string const & name,
			std::shared_ptr<TrueTypeFontManager> ttfManager,
			std::string const & fontName,
			std::string const & text,
			int const size,
			SDL_Color const & color);

		void addImageTexture(
			std::string const & name,
			std::string const & path);

		bool hasTexture(std::string const & name);

		void removeTexture(std::string const & name);

		void printText(std::string const & text,
				std::string const & fontName,
				int const size,
				SDL_Color const & color,
				SDL_Rect const & destination);

		void printDebugText(std::string const & fontName,
			int const size,
			int const xDest,
			int const yDest);

		Texture & getTexture(std::string const & name);

		std::array<int, 4> getBorderSize(void) const;
		float getBrightness(void) const;
		int getCurrentDisplayIndex(void) const;
		SDL_Renderer * getRenderer(void);
		SDL_Window * getAddress(void);
		Uint32 getId(void);

		void handleResize(void);
};

#endif // WINDOW_HPP_INCLUDED
