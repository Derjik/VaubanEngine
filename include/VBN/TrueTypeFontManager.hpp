#ifndef TRUE_TYPE_FONT_MANAGER_HPP_INCLUDED
#define TRUE_TYPE_FONT_MANAGER_HPP_INCLUDED

#include <map>
#include <set>
#include <string>
#include <VBN/TrueTypeFont.hpp>

class TrueTypeFontManager
{
	private:
		std::set<std::string> _fontNames;
		std::map<std::pair<std::string, int>, TrueTypeFont> _fonts;

	public:
		TrueTypeFontManager(std::set<std::string> const & fontNames);
		TrueTypeFontManager(TrueTypeFontManager const &) = delete;
		TrueTypeFontManager(TrueTypeFontManager &&) = delete;
		TrueTypeFontManager & operator = (TrueTypeFontManager const &) = delete;
		TrueTypeFontManager & operator = (TrueTypeFontManager &&) = delete;

		// May throw
		TrueTypeFont * getFont(std::string const & name, int const size);
};

#endif // TRUE_TYPE_FONT_MANAGER_HPP_INCLUDED
