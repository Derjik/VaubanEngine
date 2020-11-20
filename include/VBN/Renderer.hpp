#ifndef RENDERER_HPP_INCLUDED
#define RENDERER_HPP_INCLUDED

#include <memory>
#include <SDL2/SDL_render.h>
#include <VBN/Texture.hpp>
#include <VBN/BitmapFontManager.hpp>

/*!
 * SDL_Renderer wrapper class
 *
 * Each instance of this class encapsulates an SDL_Renderer object in a
 * dedicated std::unique_ptr, configured to free the underlying memory chunk
 * using SDL_DestroyRenderer().
 */
class Renderer
{
	private:
		//! Underlying SDL_Renderer enclosed in std::unique_ptr
		std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> _renderer;
		//! BitmapFontManager instance associated with the Renderer
		std::unique_ptr<BitmapFontManager> _bitmapFontManager;
		//! TrueTypeFontManager instance associated with the Renderer
		std::shared_ptr<TrueTypeFontManager> _trueTypeFontManager;
		//! Map of named Texture objects available to copy on the Renderer
		std::map<std::string, Texture> _textures;

	public:
		//! Build a Renderer for an existing Window
		Renderer(
			SDL_Window * window,
			Uint32 flags,
			std::shared_ptr<TrueTypeFontManager> ttfManager);
		//! Delete a Renderer
		~Renderer(void);
		Renderer(Renderer const &) = delete;
		Renderer & operator = (Renderer const &) = delete;

		//! Set current drawing color (separate components)
		void setDrawColor(
			Uint8 const red,
			Uint8 const green,
			Uint8 const blue,
			Uint8 const alpha);
		//! Set current drawing color (SDL_Color aggregate)
		void setDrawColor(SDL_Color const & color);
		//! Set current blending mode
		void setBlendMode(SDL_BlendMode const & blendMode);
		//! Set current rendering canvas size
		void setLogicalSize(int const w, int const h);
		//! Set current viewport
		void setViewport(SDL_Rect const & viewport);
		//! Reset viewport to whole surface
		void resetViewport(void);
		//! Set current rendering scale
		void setScale(float const x, float const y);

		//! Build a Texture from Latin1-encoded text and store it
		void addLatin1TextTexture(
			std::string const & name,
			std::string const & fontName,
			std::string const & text,
			int const size,
			SDL_Color const & color);

		//! Build a Texture from UTF-8-encoded text and store it
		void addUTF8TextTexture(
			std::string const & name,
			std::string const & fontName,
			std::string const & text,
			int const size,
			SDL_Color const & color);

		//! Build a Texture from an image and store it
		void addImageTexture(
			std::string const & name,
			std::string const & path);

		//! Get named Texture
		Texture * getTexture(std::string const & name);

		//! Clear rendering surface
		void clear(void);
		//! Fill rendering surface with current drawing color
		void fill(void);
		//! Fill rectangle with current drawng color
		void fillRect(SDL_Rect const & rectangle);
		//! Draw rectangle with current drawng color
		void drawRect(SDL_Rect const & rectangle);
		//! Draw line with current drawng color
		void drawLine(
			int const x1,
			int const y1,
			int const x2,
			int const y2);

		//! Copy named Texture's clip to destination rectangle
		void copy(
			std::string const & textureName,
			std::string const & clipName,
			SDL_Rect const & destination);

		//! Copy named Texture's clip to destination rectangle (extended)
		void copyEx(
			std::string const & textureName,
			std::string const & clipName,
			SDL_Rect const & destination,
			double const angle,
			SDL_Point const & center,
			SDL_RendererFlip const & flip);

		//! Print a dynamically-rendered text into the destination rectangle
		void printText(std::string const & text,
				std::string const & fontName,
				int const size,
				SDL_Color const & color,
				SDL_Rect const & destination);

		//! Print the whole "debug Texture" for a given {font,size}
		void printDebugText(std::string const & fontName,
			int const size,
			int const xDest,
			int const yDest);

		//! Present current render onto screen
		void present(void);
};

#endif // RENDERER_HPP_INCLUDED
