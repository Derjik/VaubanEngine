#ifndef TEXTURE_HPP_INCLUDED
#define TEXTURE_HPP_INCLUDED

#include <memory>
#include <string>
#include <SDL2/SDL_render.h>
#include <VBN/Surface.hpp>

class Texture
{
	private:
		std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> _rawTexture;

		std::map<std::string, SDL_Rect> _clips;

		Uint32 _pixelFormat;
		int _access;
		int _width;
		int _height;

		Texture(SDL_Texture * rawTexture);

	public:
		/* Constructors */
		Texture(Texture && other);
		Texture(Texture const & other) = delete;
		Texture & operator = (Texture const &) = delete;
		Texture & operator = (Texture && other);
		~Texture(void);

		/* Intrinsic and invariable attributes */
		SDL_Texture * getSDLTexture(void);
		int getWidth(void) const;
		int getHeight(void) const;
		int getAccess(void) const;
		Uint32 getPixelFormat(void) const;

		/* Clips used for sprite rendering */
		// May throw
		void addClip(std::string const & clipName,
			SDL_Rect const & clip);
		SDL_Rect * getClip(std::string const & clipName);

		/* Alpha modulation */
		void setColorAlphaMod(SDL_Color const & color);
		SDL_Color getColorAlphaMod(void) const;

		/* Factories */
		// May throw
		static Texture fromText(
			std::shared_ptr<TrueTypeFontManager> ttfManager,
			SDL_Renderer * renderer,
			std::string const & text,
			std::string const & textFontName,
			int const textSize,
			SDL_Color const & textColor);
		// May throw
		static Texture fromSurface(
			SDL_Renderer * renderer,
			Surface & surface);
		// May throw
		static Texture fromScratch(
			SDL_Renderer * renderer,
			Uint32 const format,
			int const access,
			int const width,
			int const height);
};

#endif // TEXTURE_HPP_INCLUDED
