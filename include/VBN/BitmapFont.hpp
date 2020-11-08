#ifndef BITMAP_FONT_HPP_INCLUDED
#define BITMAP_FONT_HPP_INCLUDED

#include <string>
#include <array>
#include <VBN/Texture.hpp>
#include <VBN/TrueTypeFont.hpp>

class TrueTypeFontManager;

class BitmapFont
{
	private:
		// Raw SDL_Renderer on which the BitmapFont is built
		SDL_Renderer * _sdlRenderer;

		// Available characters for this font texture
		std::string _alphabet;
		// Font tileset
		Texture _texture;

		/* TTF characteristics */
		// Metrics for each character
		std::array<TrueTypeFont::GlyphMetrics, UCHAR_MAX> _glyphMetrics;
		// Clipping rectangle to render each character
		std::array<SDL_Rect, UCHAR_MAX> _clips;
		// Max height for one line
		int _lineSkip;

		/*
		 * Internal method to compute max number of chars we can put into one
		 * line, whose width (in pixel) is passed as a parameter
		 */
		unsigned int computeLineEnd(
			std::string const & text,
			unsigned int startPoint,
			unsigned int maxWidthPx) const;

	public:
		/* Constructors & destructor */
		// May throw
		BitmapFont(std::shared_ptr<TrueTypeFontManager> ttfManager,
			std::string const & name, int size,
			SDL_Renderer * renderer);
		BitmapFont(BitmapFont const & other) = delete;
		BitmapFont(BitmapFont && other);
		BitmapFont & operator = (BitmapFont const &) = delete;
		BitmapFont & operator = (BitmapFont &&) = delete;
		~BitmapFont(void);

		/* Rendering methods */
		// Render text on attached renderer using destination rectangle
		void renderText(std::string const & text,
			SDL_Color const & color,
			SDL_Rect const & destination);
		// Render the whole font texture at destination coordinates
		void renderDebug(
			int const xDest,
			int const yDest);

		/* Getters */
		// Get the whole font texture
		Texture const * getTexture(void);
		// Get all character clips
		std::array<SDL_Rect, UCHAR_MAX> getClips(void) const;
};

#endif // BITMAP_FONT_HPP_INCLUDED
