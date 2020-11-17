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
		std::shared_ptr<TrueTypeFontManager> _trueTypeFontManager;
		std::map<std::string, Texture> _textures;

	public:
		/* Constructors */
		// May throw
		Renderer(SDL_Window * window,
			Uint32 flags,
			std::shared_ptr<TrueTypeFontManager> ttfManager);
		Renderer(Renderer const &) = delete;
		Renderer & operator = (Renderer const &) = delete;
		~Renderer(void);

		/* Renderer settings */
		void setDrawColor(
			Uint8 const red,
			Uint8 const green,
			Uint8 const blue,
			Uint8 const alpha);
		void setDrawColor(SDL_Color const & color);
		void setBlendMode(
			SDL_BlendMode const & blendMode);
		void setLogicalSize(
			int const w,
			int const h);
		void setViewport(
			SDL_Rect const & viewport);
		void resetViewport(void);
		void setScale(float const x, float const y);

		/* Texture management */
		// May throw
		void addLatin1TextTexture(
			std::string const & name,
			std::string const & fontName,
			std::string const & text,
			int const size,
			SDL_Color const & color);
		// May throw
		void addUTF8TextTexture(
			std::string const & name,
			std::string const & fontName,
			std::string const & text,
			int const size,
			SDL_Color const & color);
		// May throw
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

		void copyEx(
			std::string const & textureName,
			std::string const & clipName,
			SDL_Rect const & destination,
			double const angle,
			SDL_Point const & center,
			SDL_RendererFlip const & flip);

		// May throw
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
