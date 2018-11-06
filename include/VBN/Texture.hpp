#ifndef TEXTURE_HPP_INCLUDED
#define TEXTURE_HPP_INCLUDED

#include <memory>
#include <string>
#include <SDL2/SDL_render.h>
#include <VBN/Surface.hpp>

class Texture
{
	private:
		struct SDLTextureDeleter { void operator() (SDL_Texture * texture) {} };
		std::unique_ptr<SDL_Texture, SDLTextureDeleter> _rawTexture;

		std::map<std::string, std::unique_ptr<SDL_Rect>> _clips;

		Uint32 _pixelFormat;
		int _access;
		int _width;
		int _height;

		Texture(SDL_Texture * rawTexture);

	public:
		/* Allow move constructor and affectation only */
		Texture(Texture && other);
		Texture & operator = (Texture && other);
		Texture(Texture const & other) = delete;

		/* Intrinsic and invariable attributes */
		SDL_Texture * getSDLTexture(void);
		int getWidth(void) const;
		int getHeight(void) const;
		int getAccess(void) const;
		Uint32 getPixelFormat(void) const;

		/* Clips used for sprite rendering */
		void addClip(std::string const & clipName,
			SDL_Rect const & clip);
		SDL_Rect * getClip(std::string const & clipName);

		/* Alpha modulation */
		void setColorAlphaMod(SDL_Color const & color);
		SDL_Color getColorAlphaMod(void) const;

		/* Factories */
		static Texture fromText(
			std::shared_ptr<TrueTypeFontManager> ttfManager,
			SDL_Renderer * renderer,
			std::string const & text,
			std::string const & textFontName,
			int const textSize,
			SDL_Color const & textColor);
		static Texture fromSurface(
			SDL_Renderer * renderer,
			Surface & surface);
		static Texture fromScratch(
			SDL_Renderer * renderer,
			Uint32 const format,
			int const access,
			int const width,
			int const height);
};

#endif // TEXTURE_HPP_INCLUDED
