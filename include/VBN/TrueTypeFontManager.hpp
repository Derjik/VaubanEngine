#ifndef TRUE_TYPE_FONT_MANAGER_HPP_INCLUDED
#define TRUE_TYPE_FONT_MANAGER_HPP_INCLUDED

#include <map>
#include <set>
#include <string>
#include <VBN/TrueTypeFont.hpp>

/*!
 * Stores & retrieves TrueTypeFont objects
 *
 * The TrueTypeFontManager builds, stores & retrieves True Type fonts
 */
class TrueTypeFontManager
{
	private:
		//! Full or relative path to Audio Assets Directory
		std::string _assetsDirectory;
		//! Set containing all available fonts' names
		std::set<std::string> _fontNames;
		//! Cache map containing all previously instantiated {font,size} pairs
		std::map<std::pair<std::string, int>, TrueTypeFont> _fonts;

	public:
		//! Build a TrueTypeFontManager
		TrueTypeFontManager(
			std::string const & assetsDirectory,
			std::set<std::string> const & fontNames);
		//! Delete a TrueTypeFontManager
		~TrueTypeFontManager(void);
		TrueTypeFontManager(TrueTypeFontManager const &) = delete;
		TrueTypeFontManager(TrueTypeFontManager &&) = delete;
		TrueTypeFontManager & operator = (TrueTypeFontManager const &) = delete;
		TrueTypeFontManager & operator = (TrueTypeFontManager &&) = delete;

		//! Get a (cached or newly created) TrueTypeFont
		TrueTypeFont * getFont(std::string const & name, int const size);
};

#endif // TRUE_TYPE_FONT_MANAGER_HPP_INCLUDED
