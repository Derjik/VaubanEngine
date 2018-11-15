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

	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Build BitmapFontManager %p",
		this,
		_trueTypeFontManager.get(),
		renderer);
}

BitmapFontManager::BitmapFontManager(BitmapFontManager && other) :
	_renderer(std::move(other._renderer)),
	_trueTypeFontManager(other._trueTypeFontManager),
	_fonts(std::move(other._fonts))
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Move BitmapFontManager %p into new BitmapFontManager %p",
		&other,
		this);
}

BitmapFontManager::~BitmapFontManager(void)
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Delete BitmapFontManager %p",
		this);
}

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
	BitmapFont * font(nullptr);

	auto const fontIterator = _fonts.find(make_pair(name, size));
	if(fontIterator == _fonts.end())
	{
		try
		{
			BitmapFont bitmapFont(_trueTypeFontManager, name, size, _renderer);

			auto insertedPair(_fonts.emplace(
				make_pair(name, size),
				std::move(bitmapFont)));

			font = (&insertedPair.first->second);
		}
		catch (Exception const & exc)
		{
			EXCEPT(exc);
		}
	}
	else
	{
		font = (&fontIterator->second);
	}

	return font;
}