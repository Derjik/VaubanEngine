#ifndef WINDOW_HPP_INCLUDED
#define WINDOW_HPP_INCLUDED

#include <memory>
#include <string>
#include <map>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <VBN/Texture.hpp>
#include <VBN/BitmapFontManager.hpp>
#include <VBN/Renderer.hpp>

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
		std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> _window;
		std::unique_ptr<Renderer> _renderer;

		RatioType _ratioType;
		int _canvasWidth;
		int _canvasHeight;

		std::unique_ptr<BitmapFontManager> _bitmapFontManager;

		std::map<std::string, Texture> _textures;

		void applyRatioTypeSettings(void);
		void updateCanvasFrame(SDL_Rect const & outerSurface);

	public:
		// May throw
		Window(std::string const & title,
			int xPosition, int yPosition,
			int windowWidth, int windowHeight,
			RatioType ratioType,
			Uint32 windowFlags,
			Uint32 rendererFlags,
			std::shared_ptr<TrueTypeFontManager> ttfManager);
		Window(Window const & other) = delete;
		Window(Window && other);
		Window & operator = (Window const &) = delete;
		Window & operator = (Window &&) = delete;
		~Window(void);

		void toggleFullscreen(void);

		std::array<int, 4> getBorderSize(void) const;
		float getBrightness(void) const;
		int getCurrentDisplayIndex(void) const;
		Renderer * getRenderer(void);
		Uint32 getId(void);

		void handleResize(void);
};

#endif // WINDOW_HPP_INCLUDED
