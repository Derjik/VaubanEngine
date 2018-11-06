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
		std::string _alphabet;
		Texture _texture;
		std::array<TrueTypeFont::GlyphMetrics, UCHAR_MAX> _glyphMetrics;
		std::array<SDL_Rect, UCHAR_MAX> _clips;
		int _lineSkip;

	public:
		BitmapFont(std::shared_ptr<TrueTypeFontManager> ttfManager,
			std::string const & name, int size,
			SDL_Renderer * renderer);
		BitmapFont(BitmapFont const & other) = delete;
		BitmapFont(BitmapFont && other);

		void renderText(std::string const & text,
			SDL_Color const & color,
			SDL_Rect const & destination,
			SDL_Renderer * renderer);

		void renderDebug(SDL_Renderer * renderer,
				int const xDest,
				int const yDest);

		Texture & getTexture(void);
		std::array<SDL_Rect, UCHAR_MAX> getClips(void) const;
};

#endif // BITMAP_FONT_HPP_INCLUDED
