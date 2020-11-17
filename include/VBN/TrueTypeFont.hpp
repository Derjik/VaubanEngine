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
		/* Internal SDL TTF_Font */
		std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> _font;

	public:
		/* TTF metrics structure for a given glyph */
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

		/* Constructors & destructor */
		// May throw
		TrueTypeFont(std::string const & filePath,
			unsigned const size,
			unsigned const face = 0);
		TrueTypeFont(TrueTypeFont &&other);
		TrueTypeFont(TrueTypeFont const &other) = delete;
		TrueTypeFont & operator = (TrueTypeFont const &) = delete;
		TrueTypeFont & operator = (TrueTypeFont &&) = delete;
		~TrueTypeFont();

		/* Getters */
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

		/* Setters */
		void setStyle(int const style);
		void setOutline(int const outline);
		void setHinting(int const hinting);
		void setKerning(int const kerning);

		/* Computation */
		std::pair<int, int> getTextSize(std::string const text) const;

		/* Rendering */
		// May throw
		SDL_Surface * renderSolidLatin1(
			std::string const & text,
			SDL_Color const & color);
		// May throw
		SDL_Surface * renderSolidUTF8(
			std::string const & text,
			SDL_Color const & color);
};

#endif // TRUE_TYPE_FONT_HPP_INCLUDED