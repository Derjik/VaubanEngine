#ifndef SURFACE_HPP_INCLUDED
#define SURFACE_HPP_INCLUDED

#include <memory>
#include <string>
#include <SDL2/SDL_surface.h>

class TrueTypeFontManager;

/*!
 * SDL_Surface wrapper class
 *
 * Each instance of this class encapsulates an SDL_Surface object in a dedicated
 * std::unique_ptr, configured to free the underlying memory chunk using
 * SDL_FreeSurface().
 */
class Surface
{
	private:
		//! Underlying SDL_Surface enclosed in std::unique_ptr
		std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> _rawSurface;

		//! Private constructor (use factories for public instantiation)
		Surface(SDL_Surface * rawSurface);

	public:
		//! Move a Surface instance
		Surface(Surface && other);
		//! Delete a Surface instance
		~Surface(void);

		Surface(Surface const & other) = delete;
		Surface & operator = (Surface const &) = delete;

		//! Get raw pointer to underlying SDL_Surface
		SDL_Surface * getSurface(void);

		//! Print a Latin1-encoded string onto a new Surface
		static Surface fromLatin1Text(
			std::shared_ptr<TrueTypeFontManager> ttfManager,
			std::string const & text,
			std::string const & fontName,
			int const size,
			SDL_Color const & color);

		//! Print an UTF-8-encoded string onto a new Surface
		static Surface fromUTF8Text(
			std::shared_ptr<TrueTypeFontManager> ttfManager,
			std::string const & text,
			std::string const & fontName,
			int const size,
			SDL_Color const & color);

		//! Build a Surface from a disk-stored image
		static Surface fromImage(std::string const & path);

		//! Build a Surface from scratch using SDL parameters
		static Surface fromScratch(
			int const width,
			int const height,
			int const depth);
};

#endif // SURFACE_HPP_INCLUDED