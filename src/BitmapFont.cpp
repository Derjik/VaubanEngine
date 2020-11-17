#include <VBN/BitmapFont.hpp>
#include <climits>
#include <cctype>
#include <vector>
#include <VBN/Window.hpp>
#include <VBN/WindowManager.hpp>
#include <VBN/Engine.hpp>
#include <VBN/TrueTypeFontManager.hpp>
#include <VBN/Surface.hpp>
#include <VBN/Logging.hpp>
#include <VBN/Exceptions.hpp>

BitmapFont::BitmapFont(
	std::shared_ptr<TrueTypeFontManager> ttfManager,
	std::string const & name,
	int size,
	SDL_Renderer * renderer) :
	_sdlRenderer(renderer),
	_texture(Texture::fromScratch(renderer,
				SDL_PIXELFORMAT_RGBA32,
				SDL_TEXTUREACCESS_STATIC,
				10, 10))
{
	if (!ttfManager)
		THROW(Exception, "Received nullptr 'ttfManager'");
	if (name.empty())
		THROW(Exception, "Received empty 'name'");
	if (size <= 0)
		THROW(Exception, "Received 'size' <= 0");
	if (_sdlRenderer == nullptr)
		THROW(Exception, "Received nullptr 'renderer'");

	TrueTypeFont * font = ttfManager->getFont(name, size);
	if (!font)
		THROW(Exception,
			"Cannot retrieve font '%s' size '%d'",
			name.c_str(),
			size);

	_lineSkip = font->getLineSkip();
	TrueTypeFont::GlyphMetrics spaceMetrics = font->getGlyphMetrics(' ');

	unsigned char currentCharacter(0);
	int currentLine(0), maxWidth(0);
	std::vector<Surface> lineSurfaces;

	/* Iterate through available alphabet, building 16 char long lines */
	do
	{
		std::string lineString;
		int	horizontalAdvance(0),
			currentPrintable(0),
			verticalAdvance(currentLine * _lineSkip);

		do
		{
			/* Skip unprintable characters */
			if (std::isprint(currentCharacter))
			{
				lineString.append(1, currentCharacter);
				_glyphMetrics[currentCharacter] =
					font->getGlyphMetrics(currentCharacter);
				_clips[currentCharacter] =
					{horizontalAdvance + _glyphMetrics[currentCharacter].xMin,
					verticalAdvance,
					_glyphMetrics[currentCharacter].width,
					_lineSkip};

				horizontalAdvance +=
					_glyphMetrics[currentCharacter].advance;

				++currentPrintable;

				/*
				VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
					"'%c' x(%d;%d)y(%d;%d)a(%d)",
					currentCharacter,
					_glyphMetrics[currentCharacter].xMin,
					_glyphMetrics[currentCharacter].xMax,
					_glyphMetrics[currentCharacter].yMin,
					_glyphMetrics[currentCharacter].yMax,
					_glyphMetrics[currentCharacter].advance);
					*/
			}
			else
			{
				_clips[currentCharacter] = {0, 0, 0, 0};
				_glyphMetrics[currentCharacter] = {0, 0, 0, 0, 0};
			}

			++currentCharacter;
		} while(currentPrintable < 16 && currentCharacter < UCHAR_MAX);

		/* Generate a surface that contains the current 16 character line */
		lineSurfaces.push_back(Surface::fromLatin1Text(
								ttfManager, lineString, name, size,
								{255,255,255,255}));

		/* Update longest line width */
		Surface & lastLineInserted(lineSurfaces.back());
		if (lastLineInserted.getSurface()->w > maxWidth)
			maxWidth = lastLineInserted.getSurface()->w;

		++currentLine;
	} while(currentCharacter < UCHAR_MAX);

	/* Instantiate internal target surface */
	Surface internalSurface = Surface::fromScratch(maxWidth,
												_lineSkip*currentLine, 32);

	/* Blit previously generated line surfaces on main surface */
	int line(0);
	for(auto & surface : lineSurfaces)
	{
		SDL_Rect lineDest{0,
				line*_lineSkip,
				internalSurface.getSurface()->w,
				_lineSkip};

		SDL_BlitSurface(surface.getSurface(), nullptr,
						internalSurface.getSurface(), &lineDest);

		++line;
	}

	/* Convert surface into a texture for accelerated rendering */
	// TODO : memleak check
	_texture = std::move(Texture::fromSurface(renderer, internalSurface));

	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Build BitmapFont %p",
		this);
}

BitmapFont::BitmapFont(BitmapFont && other) :
	_sdlRenderer(std::move(other._sdlRenderer)),
	_alphabet(std::move(other._alphabet)),
	_texture(std::move(other._texture)),
	_glyphMetrics(std::move(other._glyphMetrics)),
	_clips(std::move(other._clips)),
	_lineSkip(std::move(other._lineSkip))
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Move BitmapFont %p into new BitmapFont %p",
		&other,
		this);
}

BitmapFont::~BitmapFont(void)
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Delete BitmapFont %p",
		this);
}

unsigned int BitmapFont::computeLineEnd(
	std::string const & text,
	unsigned int cursor,
	unsigned int maxWidth) const
{
	unsigned int width(0), lastWordEnd(cursor), result(0);
	char currentCharacter(0), previousCharacter(0);

	if (cursor >= text.length())
		return text.length();

	/* Browse characters until either of those conditions are met :
	 * - End of text
	 * - Max width has been reached or exceeded
	 * - '\n' found
	 */
	while (cursor < text.length() && width < maxWidth)
	{
		currentCharacter = text.at(cursor);
		/* Special case : newline character */
		if (currentCharacter == '\n')
			return cursor;

		width += _glyphMetrics[currentCharacter].advance;
		if (cursor > 0)
		{
			previousCharacter = text.at(cursor - 1);
			if (std::isspace(currentCharacter) &&
				!std::isspace(previousCharacter))
				lastWordEnd = cursor - 1;
		}
		++cursor;
	}

	/* Handle pixel width overflow */
	if (width > maxWidth)
		result = cursor - 2;
	else
		result = cursor - 1;

	/* If we're not at the end of the text */
	if (result + 1 < text.length())
	{
		/* We're cutting the line right in the middle of a word */
		if(!std::isspace(text.at(result + 1)))
			/* Finish the line at the last detected word's end instead */
			result = lastWordEnd;
	}

	return result;
}

void BitmapFont::renderText(std::string const & text,
	SDL_Color const & color,
	SDL_Rect const & destination)
{
	int	maxLineWidth(destination.w),
		maxLines(destination.h / _lineSkip);
	if (maxLines < 1 || maxLineWidth < 1)
		return;

	/* -----------8<----------- DEBUG -----------8<----------- */
	/* Draw destination rectangle */
	if (SDL_SetRenderDrawColor(_sdlRenderer, 128, 0, 255, 255))
		ERROR(SDL_LOG_CATEGORY_APPLICATION,
			"Could not set renderer color : SDL error '%s'",
			SDL_GetError());
	if (SDL_RenderDrawRect(_sdlRenderer, &destination))
		ERROR(SDL_LOG_CATEGORY_APPLICATION,
			"Could not render rectangle : SDL error '%s'",
			SDL_GetError());
	/* ----------8<---------- END DEBUG ----------8<---------- */

	/* Apply color and alpha modulation */
	_texture.setColorAlphaMod(color);

	/* First and last character for each line */
	std::vector<std::pair<int, int>> lines;

	/* Line computation loop */
	unsigned int lineNumber(0), lineBegin(0), lineEnd(0);
	do
	{
		/* Skip any whitespace prefixing the line */
		while (lineBegin < text.length() && std::isspace(text.at(lineBegin)))
			++lineBegin;

		/* Compute line end using internal method */
		lineEnd = computeLineEnd(text, lineBegin, maxLineWidth);
		lines.push_back(std::make_pair(lineBegin, lineEnd));

		/* Increment line number & prepare next line beginning (if any) */
		++lineNumber;
		lineBegin = lineEnd + 1;
	} while(lineNumber < maxLines && lineBegin < text.length());


	/* Lines rendering loop */
	bool error(false);
	for(auto lineIterator = lines.begin() ;
		lineIterator != lines.end() ;
		++lineIterator)
	{
		std::string substring =
			text.substr(lineIterator->first,
				lineIterator->second - lineIterator->first +1);

		int lineNumber = lineIterator - lines.begin();

		int currentAdvance(0);
		for(char c : substring)
		{
			TrueTypeFont::GlyphMetrics metrics = _glyphMetrics[c];
			SDL_Rect source = _clips[c];
			SDL_Rect glyphDest{destination.x + currentAdvance + _glyphMetrics[c].xMin,
						destination.y + lineNumber*_lineSkip,
						metrics.width,
						_lineSkip};

			error |= (SDL_RenderCopy(_sdlRenderer,
						_texture.getSDLTexture(),
						&source,
						&glyphDest) != 0);

			currentAdvance += metrics.advance;
		}
	}

	if (error)
	{
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"BitmapFont::renderText: "
			"Some SDL call returned error '%s'",
			SDL_GetError());
	}
}

void BitmapFont::renderDebug(
	int const xDest,
	int const yDest)
{
	SDL_Rect dest{xDest, yDest, _texture.getWidth(), _texture.getHeight()};
	SDL_SetRenderDrawColor(_sdlRenderer, 255, 255, 255, 255);
	SDL_RenderCopy(_sdlRenderer, _texture.getSDLTexture(), nullptr, &dest);

	SDL_SetRenderDrawColor(_sdlRenderer, 255, 69, 0, 255);
	for(auto rect : _clips)
	{
		rect.x += xDest;
		rect.y += yDest;
		SDL_RenderDrawRect(_sdlRenderer, &rect);
	}
}

std::array<SDL_Rect, UCHAR_MAX> BitmapFont::getClips(void) const
{
	return _clips;
}

Texture const * BitmapFont::getTexture(void)
{
	return &_texture;
}