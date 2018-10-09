#ifndef SURFACE_HPP_INCLUDED
#define SURFACE_HPP_INCLUDED

#include <memory>
#include <string>
#include <SDL2/SDL_surface.h>
#include <VBN/TrueTypeFontManager.hpp>

struct SDLSurfaceDeleter { void operator () (SDL_Surface * surface) const; };

class Surface
{
	private:
		std::unique_ptr<SDL_Surface, SDLSurfaceDeleter> _rawSurface;
		Surface(SDL_Surface * rawSurface);

	public:
		Surface(Surface const & other) = delete;
		Surface(Surface && other);
		Surface & operator = (Surface && other);

		SDL_Surface * getSurface(void);

		static Surface fromText(
			std::shared_ptr<TrueTypeFontManager> ttfManager,
			std::string const & text,
			std::string const & fontName,
			int const size,
			SDL_Color const & color);

		static Surface fromImage(
			std::string const & path);

		static Surface fromScratch(
			int const width,
			int const height,
			int const depth);
};

#endif // SURFACE_HPP_INCLUDED