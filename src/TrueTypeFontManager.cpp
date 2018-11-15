#include <VBN/TrueTypeFontManager.hpp>
#include <VBN/Logging.hpp>
#include <VBN/Exceptions.hpp>

TrueTypeFontManager::TrueTypeFontManager(
	std::set<std::string> const & fontNames) :
	_fontNames(fontNames)
{}

TrueTypeFont * TrueTypeFontManager::getFont(
	std::string const & name,
	int const size)
{
	if (name.empty())
		THROW(Exception, "Received empty 'name'");
	if (size <= 0)
		THROW(Exception, "Received 'size' <= 0");
	if (_fontNames.find(name) == _fontNames.end())
		THROW(Exception,
			"No '%s' font configured",
			name.c_str());

	TrueTypeFont * font(nullptr);

	auto const fontIterator = _fonts.find(make_pair(name, size));
	if(fontIterator == _fonts.end())
	{
		DEBUG(SDL_LOG_CATEGORY_APPLICATION,
			"Font '%s' size '%d' not loaded, loading",
			name.c_str(), size);

		try
		{
			TrueTypeFont ttFont(name + ".ttf", size);

			auto insertedPair(_fonts.emplace(
				make_pair(name, size),
				std::move(ttFont)));

			font = (&insertedPair.first->second);
		}
		catch (Exception const & exc)
		{
			EXCEPT(exc);
		}
	}
	else
	{
		DEBUG(SDL_LOG_CATEGORY_APPLICATION,
			"Font '%s' size '%d' already loaded, reusing",
			name.c_str(), size);

		font = &fontIterator->second;
	}

	return font;
}