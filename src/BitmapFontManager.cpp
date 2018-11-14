#include <VBN/BitmapFontManager.hpp>
#include <VBN/TrueTypeFontManager.hpp>
#include <VBN/Logging.hpp>
#include <VBN/Exceptions.hpp>

BitmapFontManager::BitmapFontManager(
	std::shared_ptr<TrueTypeFontManager> trueTypeFontManager,
	SDL_Renderer * renderer) :
	_renderer(renderer),
	_trueTypeFontManager(trueTypeFontManager)
{
	if (!trueTypeFontManager)
		THROW(Exception, "Received nullptr 'trueTypeFontManager'");
	if(!_renderer)
		THROW(Exception, "Received nullptr 'renderer'");
}

BitmapFontManager::BitmapFontManager(BitmapFontManager && other) :
	_renderer(std::move(other._renderer)),
	_trueTypeFontManager(other._trueTypeFontManager),
	_fonts(std::move(other._fonts))
{}

void BitmapFontManager::preload(
	std::vector < std::pair<std::string, int>> const fonts)
{
	for(auto const & fontConfig : fonts)
	{
		DEBUG(SDL_LOG_CATEGORY_APPLICATION,
			"BitmapFontManager::preload: preloading font "
			"'%s' size %d",
			fontConfig.first.c_str(),
			fontConfig.second);

		getFont(fontConfig.first, fontConfig.second);
	}
}

BitmapFont * BitmapFontManager::getFont(std::string const & name, int const size)
{
	auto const fontIterator = _fonts.find(make_pair(name, size));
	if(fontIterator == _fonts.end())
	{
		_fonts.emplace(make_pair(name, size),
			BitmapFont(_trueTypeFontManager, name, size, _renderer));

		return (&_fonts.at(make_pair(name, size)));
	}
	else
	{
		return (&fontIterator->second);
	}
}