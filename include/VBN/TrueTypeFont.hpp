#ifndef TRUE_TYPE_FONT_HPP_INCLUDED
#define TRUE_TYPE_FONT_HPP_INCLUDED

#include <memory>
#include <string>
#include <utility>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_surface.h>

class TrueTypeFont
{
	private:
		std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> _font;

	public:
		struct GlyphMetrics
		{
			int xMin;
			int yMin;
			int xMax;
			int yMax;
			int width;
			int height;
			int advance;
		};

		TrueTypeFont(std::string const & filePath,
			unsigned const size,
			unsigned const face = 0);
		TrueTypeFont(TrueTypeFont &&other);
		TrueTypeFont(TrueTypeFont const &other) = delete;
		~TrueTypeFont();

		TTF_Font const * getFont(void) const;

		int getStyle(void) const;
		int getOutline(void) const;
		int getHinting(void) const;
		int getKerning(void) const;

		int getAscent(void) const;
		int getDescent(void) const;
		int getLineSkip(void) const;
		int getFaces(void) const;
		bool getIsFixedWidth(void) const;
		std::string getFaceFamilyName(void) const;
		
		GlyphMetrics getGlyphMetrics(char const c) const;

		void setStyle(int const style);
		void setOutline(int const outline);
		void setHinting(int const hinting);
		void setKerning(int const kerning);

		std::pair<int, int> getTextSize(std::string const text) const;

		SDL_Surface * renderSolid(std::string const & text,
					SDL_Color const & color);
};

#endif // TRUE_TYPE_FONT_HPP_INCLUDED