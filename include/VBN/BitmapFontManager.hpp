#ifndef BITMAP_FONT_MANAGER_HPP_INCLUDED
#define BITMAP_FONT_MANAGER_HPP_INCLUDED

#include <map>
#include <string>
#include <vector>
#include <VBN/BitmapFont.hpp>

class TrueTypeFontManager;

class BitmapFontManager
{
	private:
		SDL_Renderer * _renderer;
		std::shared_ptr<TrueTypeFontManager> _trueTypeFontManager;
		std::map<std::pair<std::string, int>, BitmapFont> _fonts;

	public:
		BitmapFontManager(std::shared_ptr<TrueTypeFontManager> trueTypeFontManager,
							SDL_Renderer * renderer);
		BitmapFontManager(BitmapFontManager const & other) = delete;
		BitmapFontManager(BitmapFontManager && other);
		BitmapFontManager & operator = (BitmapFontManager const &) = delete;
		BitmapFontManager & operator = (BitmapFontManager &&) = delete;
		~BitmapFontManager(void);

		void preload(std::vector<std::pair<std::string, int>> const fonts);
		BitmapFont * getFont(std::string const & name, int const size);
};

#endif // BITMAP_FONT_MANAGER_HPP_INCLUDED
