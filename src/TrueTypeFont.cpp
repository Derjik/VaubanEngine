#include <VBN/TrueTypeFont.hpp>

TrueTypeFont::TrueTypeFont(std::string const & filePath,
		unsigned const size,
		unsigned const face) : _font(nullptr)
{
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
		"TrueTypeFont: file %s, size %d, face %d,",
		filePath.c_str(),
		size,
		face);

	_font = std::unique_ptr<TTF_Font, TTFDeleter>(
		TTF_OpenFontIndex(filePath.c_str(), size, face), TTFDeleter());

	if(_font == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			"TrueTypeFont: TTF_OpenFontIndex returned error '%s'",
			TTF_GetError());
		throw std::string(
			"TrueTypeFont: TTF_OpenFontIndex returned error '"
			+ std::string(TTF_GetError()) + "'");
	}
	else
	{
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
			"TrueTypeFont: name %s ascent %d, "
			"descent %d, "
			"lineskip %d, "
			"faces %d, "
			"fixed width %d",
			getFaceFamilyName().c_str(),
			getAscent(),
			getDescent(),
			getLineSkip(),
			getFaces(),
			getIsFixedWidth());

		TTF_SetFontKerning(_font.get(), 0);
	}
}

TrueTypeFont::TrueTypeFont(TrueTypeFont && other) : _font(std::move(other._font))
{}

TrueTypeFont::~TrueTypeFont()
{}

TTF_Font const * TrueTypeFont::getFont(void) const
{
	return _font.get();
}

int TrueTypeFont::getStyle(void) const
{
	return TTF_GetFontStyle(_font.get());
}

int TrueTypeFont::getOutline(void) const
{
	return TTF_GetFontOutline(_font.get());
}

int TrueTypeFont::getHinting(void) const
{
	return TTF_GetFontHinting(_font.get());
}

int TrueTypeFont::getKerning(void) const
{
	return TTF_GetFontKerning(_font.get());
}

int TrueTypeFont::getAscent(void) const
{
	return TTF_FontAscent(_font.get());
}

int TrueTypeFont::getDescent(void) const
{
	return TTF_FontDescent(_font.get());
}

int TrueTypeFont::getLineSkip(void) const
{
	return TTF_FontLineSkip(_font.get());
}

int TrueTypeFont::getFaces(void) const
{
	return TTF_FontFaces(_font.get());
}

bool TrueTypeFont::getIsFixedWidth(void) const
{
	return TTF_FontFaceIsFixedWidth(_font.get());
}

std::string TrueTypeFont::getFaceFamilyName(void) const
{
	return TTF_FontFaceFamilyName(_font.get());
}

GlyphMetrics TrueTypeFont::getGlyphMetrics(char const c) const
{
	GlyphMetrics metrics;
	TTF_GlyphMetrics(_font.get(), c,
			&metrics.xMin, &metrics.xMax,
			&metrics.yMin, &metrics.yMax,
			&metrics.advance);
	metrics.width = metrics.xMax - metrics.xMin;
	metrics.height = metrics.yMax - metrics.yMin;
	return metrics;
}

void TrueTypeFont::setStyle(int const style)
{
	if(TTF_GetFontStyle(_font.get()) != style)
		TTF_SetFontStyle(_font.get(), style);
}

void TrueTypeFont::setOutline(int const outline)
{
	if(TTF_GetFontOutline(_font.get()) != outline)
		TTF_SetFontOutline(_font.get(), outline);
}

void TrueTypeFont::setHinting(int const hinting)
{
	if(TTF_GetFontHinting(_font.get()) != hinting)
		TTF_SetFontHinting(_font.get(), hinting);
}

void TrueTypeFont::setKerning(int const kerning)
{
	if(TTF_GetFontKerning(_font.get()) != kerning)
		TTF_SetFontKerning(_font.get(), kerning);
}

std::pair<int, int> TrueTypeFont::getTextSize(std::string const text) const
{
	std::pair<int, int> result{0,0};

	TTF_SizeText(_font.get(), text.c_str(), &result.first, &result.second);

	return result;
}

SDL_Surface * TrueTypeFont::renderSolid(std::string const & text,
					SDL_Color const & color)
{
	SDL_Surface * renderedText(TTF_RenderText_Solid(
		_font.get(), text.c_str(), color));
	if(renderedText == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
				"TrueTypeFont::renderSolid: "
				"TTF_RenderText_Solid() returned '%s'",
				TTF_GetError());
		throw std::string("TrueTypeFont::renderSolid: "
				"TTF_RenderText_Solid() returned '" +
				std::string(TTF_GetError()) + "'");
	}
	return renderedText;
}

void TTFDeleter::operator() (TTF_Font * font) const
{
	TTF_CloseFont(font);
}
