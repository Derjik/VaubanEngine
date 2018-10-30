#include <VBN/TrueTypeFontManager.hpp>
#include <VBN/Logging.hpp>
#include <VBN/Exceptions.hpp>

TrueTypeFontManager::TrueTypeFontManager(std::set<std::string> const & fontNames) : _fontNames(fontNames)
{}

TrueTypeFont &
TrueTypeFontManager::getFont(std::string const & name, int const size)
{
	if (name.empty())
		THROW(Exception, "Received empty 'name'");
	if (size <= 0)
		THROW(Exception, "Received 'size' <= 0");
	if (_fontNames.find(name) == _fontNames.end())
		THROW(Exception,
			"No '%s' font configured", name.c_str());

	auto const fontIterator = _fonts.find(make_pair(name, size));
	if(fontIterator == _fonts.end())
	{
		DEBUG(SDL_LOG_CATEGORY_APPLICATION,
			"Font '%s' size %d not loaded, loading",
			name.c_str(), size);
		_fonts.emplace(make_pair(name, size), TrueTypeFont(name + ".ttf", size));
		return _fonts.at(make_pair(name, size));
	}
	else
	{
		DEBUG(SDL_LOG_CATEGORY_APPLICATION,
			"Font '%s' size %d already loaded, reusing",
			name.c_str(), size);
		return fontIterator->second;
	}
}