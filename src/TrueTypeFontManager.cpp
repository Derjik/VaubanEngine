#include <VBN/TrueTypeFontManager.hpp>
#include <VBN/Logging.hpp>
#include <VBN/Exceptions.hpp>

/*!
 * Main constructor for TrueTypeFontManager : uses a .ttf asset directory and a
 * dictionnary of all available fonts
 *
 * @param	assetsDirectory		Relative or absolute path where all .ttf files
 *								should be retrieved
 * @param	fontNames			Set containing all available filenames
 */
TrueTypeFontManager::TrueTypeFontManager(
	std::string const & assetsDirectory,
	std::set<std::string> const & fontNames) :
	_assetsDirectory(assetsDirectory),
	_fontNames(fontNames)
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Build TrueTypeFontManager %p",
		this);
}

TrueTypeFontManager::~TrueTypeFontManager(void)
{

	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Delete TrueTypeFontManager %p",
		this);
}

/*!
 * Checks the internal cache for an existing {name,size} TTF font, creates one
 * if required, returns the result.
 *
 * @param	fontName	Name of the font (filename without the .ttf extension)
 * @param	size		Size of the font
 * @returns				Raw pointer to the corresponding font
 * @throws	Exception	Invalid input parameters or SDL TTF call error
 */
TrueTypeFont * TrueTypeFontManager::getFont(
	std::string const & fontName,
	int const size)
{
	// Check input parameters
	if (size <= 0)
		THROW(Exception,
			"Received 'size' <= 0");
	if (_fontNames.find(fontName) == _fontNames.end())
		THROW(Exception,
			"No '%s' font configured",
			fontName.c_str());

	TrueTypeFont * font(nullptr);

	// Look for pre-existing {name, size} pair in cache
	auto const fontIterator = _fonts.find(make_pair(fontName, size));

	if(fontIterator == _fonts.end()) /* Cache miss */
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
	else /* Cache hit */
	{
		VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
			"Font '%s' size '%d' already loaded, reusing",
			fontName.c_str(), size);

		// Return cached font
		font = &fontIterator->second;
	}

	return font;
}