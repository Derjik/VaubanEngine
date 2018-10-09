#include <VBN/TrueTypeFontManager.hpp>

TrueTypeFontManager::TrueTypeFontManager(std::set<std::string> const & fontNames) : _fontNames(fontNames)
{}

TrueTypeFont &
TrueTypeFontManager::getFont(std::string const & name, int const size)
{
	/* No font found for this name */
	if(_fontNames.find(name) == _fontNames.end())
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			"TrueTypeFontManager::getFont: "
			"No '%s' font configured", name.c_str());
		throw std::string("No " + name + " font configured!");
	}

	/* Found a font, let's check for size */
	auto const fontIterator = _fonts.find(make_pair(name, size));
	/* Size not loaded, load it */
	if(fontIterator == _fonts.end())
	{
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
			"TrueTypeFontManager::getFont: "
			"Font '%s' size %d not loaded, loading",
			name.c_str(), size);
		_fonts.emplace(make_pair(name, size), TrueTypeFont(name + ".ttf", size));

		return _fonts.at(make_pair(name, size));
	}
	/* Size already loaded for this font */
	else
	{
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
			"TrueTypeFontManager::getFont: "
			"Font '%s' size %d already loaded, reusing",
			name.c_str(), size);
		return fontIterator->second;
	}
}