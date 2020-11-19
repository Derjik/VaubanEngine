#include <VBN/TrueTypeFont.hpp>
#include <VBN/Logging.hpp>
#include <VBN/Exceptions.hpp>

/*!
 * @param	filePath	Full path to the .ttf file (with extension)
 * @param	size		Font size
 * @param	face		Font face identifier (defaults to 0)
 * @throws	Exception	Invalid input parameters or SDL TTF call error
 *
 * @todo	Why disable kerning ?
 */
TrueTypeFont::TrueTypeFont(
	std::string const & filePath,
	unsigned const size,
	unsigned const face) :
	_font(nullptr, &TTF_CloseFont)
{
	// Check input parameters
	if (filePath.empty())
		THROW(Exception, "Received empty 'filePath'");
	if (size <= 0)
		THROW(Exception, "Received 'size' <= 0");

	// Instantiate TTF_Font and enclose it in unique_ptr
	_font = std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>(
		TTF_OpenFontIndex(filePath.c_str(), size, face),
		TTF_CloseFont);

	// Check for TTF_OpenFontIndex errors
	if (_font == nullptr)
		THROW(Exception,
			"Cannot open font : TTF error '%s'",
			TTF_GetError());

	// Disable kerning (=monospace now)
	TTF_SetFontKerning(_font.get(), 0);

	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Build TrueTypeFont %p (TTF_Font %p, name '%s' size %d)",
		this,
		_font.get(),
		filePath.c_str(),
		size);
}

TrueTypeFont::TrueTypeFont(TrueTypeFont && other) : _font(std::move(other._font))
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Move font %p (TTF_Font %p) into new font %p",
		&other,
		_font.get(),
		this);
}

TrueTypeFont::~TrueTypeFont()
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Delete TrueTypeFont %p (TTF_Font %p)",
		this,
		_font.get());
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

TrueTypeFont::GlyphMetrics TrueTypeFont::getGlyphMetrics(char const c) const
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

/*!
 * @param	text		An std::string containing a Latin1-encoded sequence to print
 *						onto a new SDL_Surface
 * @param	color		SDL_Color to use for printing
 * @returns				Raw pointer to newly created SDL_Surface containing printed
 *						text
 * @throws	Exception	TTF call error
 */
SDL_Surface * TrueTypeFont::renderSolidLatin1(
	std::string const & text,
	SDL_Color const & color)
{
	// Try rendering
	SDL_Surface * renderedText(
		TTF_RenderText_Solid(
			_font.get(),
			text.c_str(),
			color));

	// Check for rendering errors
	if(renderedText == nullptr)
		THROW(Exception,
			"Cannot render Latin1 text : TTF error '%s'",
			TTF_GetError());

	// Return rendered SDL_Surface
	return renderedText;
}

/*!
 * @param	text		An std::string containing an UTF8-encoded sequence to
 *						print onto a new SDL_Surface
 * @param	color		SDL_Color to use for printing
 * @returns				Raw pointer to newly created SDL_Surface containing printed
 *						text
 * @throws	Exception	TTF call error
 */
SDL_Surface * TrueTypeFont::renderSolidUTF8(
	std::string const & text,
	SDL_Color const & color)
{
	// Try rendering
	SDL_Surface * renderedText(
		TTF_RenderUTF8_Solid(
			_font.get(),
			text.c_str(),
			color));

	// Check for rendering errors
	if(renderedText == nullptr)
		THROW(Exception,
			"Cannot render UTF-8 text : TTF error '%s'",
			TTF_GetError());

	// Return rendered SDL_Surface
	return renderedText;
}
