#include <VBN/TrueTypeFontManager.hpp>
#include <VBN/Logging.hpp>
#include <VBN/Exceptions.hpp>

TrueTypeFontManager::TrueTypeFontManager(
	std::string const & assetsDirectory,
	std::set<std::string> const & fontNames) :
	_assetsDirectory(assetsDirectory),
	_fontNames(fontNames)
{}

TrueTypeFont * TrueTypeFontManager::getFont(
	std::string const & fontName,
	int const size)
{
	if (size <= 0)
		THROW(Exception, "Received 'size' <= 0");
	if (_fontNames.find(fontName) == _fontNames.end())
		THROW(Exception,
			"No '%s' font configured",
			fontName.c_str());

	TrueTypeFont * font(nullptr);

	auto const fontIterator = _fonts.find(make_pair(fontName, size));
	if(fontIterator == _fonts.end())
	{
		VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
			"Font '%s' size '%d' not loaded, loading",
			fontName.c_str(), size);

		// Instantiate TrueTypeFont using input file & face 0
		// May throw
		TrueTypeFont ttFont(_assetsDirectory + fontName + ".ttf", size);

		// Store newly instantiated font in cache
		auto insertedPair(_fonts.emplace(
			make_pair(fontName, size),
			std::move(ttFont)));

		// Return new font
		font = (&insertedPair.first->second);
	}
	else
	{
		VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
			"Font '%s' size '%d' already loaded, reusing",
			fontName.c_str(), size);

		font = &fontIterator->second;
	}

	return font;
}