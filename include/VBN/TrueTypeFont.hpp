#ifndef TRUE_TYPE_FONT_HPP_INCLUDED
#define TRUE_TYPE_FONT_HPP_INCLUDED

#include <memory>
#include <string>
#include <utility>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_surface.h>

/*!
 * TTF_Font wrapper class
 *
 * Each instance of this class encapsulates a TTF_Font object in a dedicated
 * std::unique_ptr, configured to free the underlying memory chunk using
 * TTF_CloseFont().
 *
 * @todo	Add documentation for TTF getters/setters
 * @todo	Implement Blended, Shaded etc. renders
 */
class TrueTypeFont
{
	private:
		//! Internal SDL TTF_Font
		std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> _font;

	public:
		/*!
		 * Aggregates all exposed metrics for a single glyph. (refer to
		 * https://www.libsdl.org/projects/SDL_ttf/docs/metrics.png for a
		 * detailed description of each field)
		 */
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

		//! Build a TrueTypeFont instance
		TrueTypeFont(
			std::string const & filePath,
			unsigned const size,
			unsigned const face = 0);
		//! Move a TrueTypeFont instance
		TrueTypeFont(TrueTypeFont &&other);
		//! Delete a TrueTypeFont instance
		~TrueTypeFont();

		TrueTypeFont(TrueTypeFont const &other) = delete;
		TrueTypeFont & operator = (TrueTypeFont const &) = delete;
		TrueTypeFont & operator = (TrueTypeFont &&) = delete;

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
		//! Render Latin1 text into an SDL_Surface
		SDL_Surface * renderSolidLatin1(
			std::string const & text,
			SDL_Color const & color);
		//! Render UTF-8 text into an SDL_Surface
		SDL_Surface * renderSolidUTF8(
			std::string const & text,
			SDL_Color const & color);
};

#endif // TRUE_TYPE_FONT_HPP_INCLUDED