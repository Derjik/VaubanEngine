#ifndef TEXTURE_HPP_INCLUDED
#define TEXTURE_HPP_INCLUDED

#include <memory>
#include <map>
#include <string>
#include <SDL2/SDL_render.h>

class TrueTypeFontManager;
class Surface;

/*!
 * SDL_Texture wrapper class
 *
 * Each instance of this class encapsulates an SDL_Texture object in a dedicated
 * std::unique_ptr, configured to free the underlying memory chunk using
 * SDL_DestroyTexture().
 */
class Texture
{
	private:
		//! Underlying SDL_Texture enclosed in std::unique_ptr
		std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> _rawTexture;

		//! Named rectangles corresponding to tiles for sprite rendering
		std::map<std::string, SDL_Rect> _clips;

		//! SDL pixel format
		Uint32 _pixelFormat;
		//! SDL access mode
		int _access;
		//! Texture width
		int _width;
		//! Texture heigt
		int _height;

		//! Private constructor (use factories for public instantiation)
		Texture(SDL_Texture * rawTexture);

	public:
		//! Move a Texture instance
		Texture(Texture && other);
		//! Assign-move a Texture instance
		Texture & operator = (Texture && other);
		//! Delete a Texture instance
		~Texture(void);
		Texture(Texture const & other) = delete;
		Texture & operator = (Texture const &) = delete;

		//! Get raw pointer to internal SDL_Texture
		SDL_Texture * getSDLTexture(void);
		//! Get Texture width
		int getWidth(void) const;
		//! Get Texture height
		int getHeight(void) const;
		//! Get Texture access mode
		int getAccess(void) const;
		//! Get Texture pixel format
		Uint32 getPixelFormat(void) const;

		
		//! Add a clipping rectangle to the clips dictionnary
		void addClip(
			std::string const & clipName,
			SDL_Rect const & clip);

		//! Get the clipping rectangle corresponding to a given name
		SDL_Rect * getClip(std::string const & clipName);

		//! Set Color-Alpha modulation
		void setColorAlphaMod(SDL_Color const & color);
		//! Get Color-Alpha modulation
		SDL_Color getColorAlphaMod(void) const;

		//! Print a Latin1-encoded string onto a new Texture instance
		static Texture fromLatin1Text(
			std::shared_ptr<TrueTypeFontManager> ttfManager,
			SDL_Renderer * renderer,
			std::string const & text,
			std::string const & textFontName,
			int const textSize,
			SDL_Color const & textColor);

		//! Print an UTF-8-encoded string onto a new Texture instance
		static Texture fromUTF8Text(
			std::shared_ptr<TrueTypeFontManager> ttfManager,
			SDL_Renderer * renderer,
			std::string const & text,
			std::string const & textFontName,
			int const textSize,
			SDL_Color const & textColor);

		//! Build a Texture instance from an existing Surface instance
		static Texture fromSurface(
			SDL_Renderer * renderer,
			Surface & surface);

		//! Build a Texture instance from scratch using SDL parameters
		static Texture fromScratch(
			SDL_Renderer * renderer,
			Uint32 const format,
			int const access,
			int const width,
			int const height);
};

#endif // TEXTURE_HPP_INCLUDED
