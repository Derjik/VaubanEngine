#ifndef SURFACE_HPP_INCLUDED
#define SURFACE_HPP_INCLUDED

#include <memory>
#include <string>
#include <SDL2/SDL_surface.h>

class TrueTypeFontManager;

class Surface
{
	private:
		std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> _rawSurface;
		// May throw
		Surface(SDL_Surface * rawSurface);

	public:
		Surface(Surface const & other) = delete;
		Surface(Surface && other);
		Surface & operator = (Surface const &) = delete;
		~Surface(void);

		SDL_Surface * getSurface(void);

		// May throw
		static Surface fromLatin1Text(
			std::shared_ptr<TrueTypeFontManager> ttfManager,
			std::string const & text,
			std::string const & fontName,
			int const size,
			SDL_Color const & color);
		// May throw
		static Surface fromUTF8Text(
			std::shared_ptr<TrueTypeFontManager> ttfManager,
			std::string const & text,
			std::string const & fontName,
			int const size,
			SDL_Color const & color);
		// May throw
		static Surface fromImage(
			std::string const & path);
		// May throw
		static Surface fromScratch(
			int const width,
			int const height,
			int const depth);
};

#endif // SURFACE_HPP_INCLUDED