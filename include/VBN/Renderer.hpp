#ifndef RENDERER_HPP_INCLUDED
#define RENDERER_HPP_INCLUDED

#include <memory>
#include <SDL2/SDL_render.h>
#include <VBN/Texture.hpp>
#include <VBN/BitmapFontManager.hpp>

class Renderer
{
	private:
		std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> _renderer;
		std::unique_ptr<BitmapFontManager> _bitmapFontManager;
		std::map<std::string, Texture> _textures;

	public:
		/* Constructors */
		Renderer(SDL_Window * window,
			int index,
			Uint32 flags,
			std::shared_ptr<TrueTypeFontManager> ttfManager);
		Renderer(Renderer const &) = delete;
		Renderer & operator = (Renderer const &) = delete;

		/* Raw SDL_Renderer access */
		//SDL_Renderer * getSDLRenderer(void);

		/* Renderer settings */
		void setDrawColor(
			Uint8 const red,
			Uint8 const green,
			Uint8 const blue,
			Uint8 const alpha);
		void setBlendMode(
			SDL_BlendMode const & blendMode);
		void setLogicalSize(
			int const w,
			int const h);
		void setViewport(
			SDL_Rect const & viewport);
		void resetViewport(void);

		/* Texture management */
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
		Texture * getTexture(std::string const & name);

		/* Drawing & printing */
		void clear(void);
		void fill(void);
		void fillRect(SDL_Rect const & rectangle);
		void drawRect(SDL_Rect const & rectangle);
		void drawLine(
			int const x1,
			int const y1,
			int const x2,
			int const y2);

		void copy(
			std::string const & textureName,
			std::string const & clipName,
			SDL_Rect const & destination);

		void printText(std::string const & text,
				std::string const & fontName,
				int const size,
				SDL_Color const & color,
				SDL_Rect const & destination);
		void printDebugText(std::string const & fontName,
			int const size,
			int const xDest,
			int const yDest);

		void present(void);
};

#endif // RENDERER_HPP_INCLUDED
