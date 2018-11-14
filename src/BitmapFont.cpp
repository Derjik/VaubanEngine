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

BitmapFont::BitmapFont(std::shared_ptr<TrueTypeFontManager> ttfManager,
						std::string const & name, int size,
						SDL_Renderer * renderer) :
						_texture(Texture::fromScratch(renderer,
									SDL_PIXELFORMAT_RGBA32,
									SDL_TEXTUREACCESS_STATIC,
									10, 10))
{
	if (!ttfManager)
		THROW(Exception, "Received nullptr 'ttfMaanager'");
	if (name.empty())
		THROW(Exception, "Received empty 'name'");
	if (size <= 0)
		THROW(Exception, "Received 'size' <= 0");
	if (renderer == nullptr)
		THROW(Exception, "Received nullptr 'renderer'");

	TrueTypeFont & font = ttfManager->getFont(name, size);

	_lineSkip = font.getLineSkip();
	TrueTypeFont::GlyphMetrics spaceMetrics = font.getGlyphMetrics(' ');

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
			if(std::isprint(currentCharacter))
			{
				lineString.append(1, currentCharacter);
				_glyphMetrics[currentCharacter] =
					font.getGlyphMetrics(currentCharacter);
				_clips[currentCharacter] =
					{horizontalAdvance + _glyphMetrics[currentCharacter].xMin,
					verticalAdvance,
					_glyphMetrics[currentCharacter].width,
					_lineSkip};

				horizontalAdvance +=
					_glyphMetrics[currentCharacter].advance;

				++currentPrintable;

				VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
					"'%c' x(%d;%d)y(%d;%d)a(%d)",
					currentCharacter,
					_glyphMetrics[currentCharacter].xMin,
					_glyphMetrics[currentCharacter].xMax,
					_glyphMetrics[currentCharacter].yMin,
					_glyphMetrics[currentCharacter].yMax,
					_glyphMetrics[currentCharacter].advance);
			}
			else
			{
				_clips[currentCharacter] = {0, 0, 0, 0};
				_glyphMetrics[currentCharacter] = {0, 0, 0, 0, 0};
			}

			++currentCharacter;
		} while(currentPrintable < 16 && currentCharacter < UCHAR_MAX);

		/* Generate a surface that contains the current 16 character line */
		lineSurfaces.push_back(Surface::fromText(
								ttfManager, lineString, name, size,
								{255,255,255,255}));

		/* Update longest line width */
		Surface & lastLineInserted(lineSurfaces.back());
		if(lastLineInserted.getSurface()->w > maxWidth)
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
	_texture = Texture::fromSurface(renderer, internalSurface);
}

BitmapFont::BitmapFont(BitmapFont && other) :
	_alphabet(std::move(other._alphabet)),
	_texture(std::move(other._texture)),
	_glyphMetrics(std::move(other._glyphMetrics)),
	_clips(std::move(other._clips)),
	_lineSkip(std::move(other._lineSkip))
{}

void BitmapFont::renderText(std::string const & text,
	SDL_Color const & color,
	SDL_Rect const & destination,
	SDL_Renderer * renderer)
{
	int	maxLength(destination.w),
		maxLines(destination.h / _lineSkip);
	if(maxLines < 1 || maxLength < 1)
		return;

	/* DEBUG */
	/* Draw destination rectangle */
	SDL_SetRenderDrawColor(renderer, 128, 0, 255, 255);
	SDL_RenderDrawRect(renderer, &destination);

	/* Apply color and alpha modulation */
	_texture.setColorAlphaMod(color);

	std::vector<std::pair<int, int>> lines;
	unsigned int index(0);

	int	lineAdvance(0),
		lineIndex(0),
		lineBegin(0),
		lineEnd(0),
		lastWordBegin(0);

	/* Process N lines */
	do
	{
		bool onWord = false;
		lineAdvance = 0;

		while(index < text.length() && lineAdvance < maxLength)
		{
			char currentCharacter = text.at(index);

			/* Beginning a word */
			if(!onWord && !std::isspace(currentCharacter))
			{
				onWord = true;
				lastWordBegin = index;
			}
			/* Ending a word */
			else if(std::isspace(currentCharacter))
			{
				onWord = false;
				lineEnd = index-1;
			}
			/* Ending the whole base string */
			else if(index == text.length() - 1)
				lineEnd = index;

			lineAdvance += _glyphMetrics[currentCharacter].advance;
			++index;

			VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
				"L%d(%d->%d) %c '%s'",
				lineIndex,
				lineBegin,
				lineEnd,
				currentCharacter,
				text.substr(lineBegin, lineEnd-lineBegin+1).c_str());
		}

		/* Line is too large to be displayed in destination rectangle */
		if(lineEnd == lineBegin)
			return;

		lines.push_back(std::make_pair(lineBegin, lineEnd));
		++lineIndex;

		/* If we were on a word when line cut occurred */
		if(onWord)
			lineBegin = lastWordBegin;
		else
			lineBegin = index;
	} while(lineIndex < maxLines && index < text.length());

	bool error(false);
	for(auto lineIterator = lines.begin() ;
		lineIterator != lines.end() ;
		++lineIterator)
	{
		std::string tempo =
			text.substr(lineIterator->first,
				lineIterator->second - lineIterator->first +1);

		int lineNumber = lineIterator - lines.begin();

		VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
			"L%d(%d->%d) '%s'",
			lineNumber,
			lineIterator->first,
			lineIterator->second,
			tempo.c_str());

		int currentAdvance(0);
		for(char c : tempo)
		{
			TrueTypeFont::GlyphMetrics metrics = _glyphMetrics[c];
			SDL_Rect source = _clips[c];
			SDL_Rect glyphDest{destination.x + currentAdvance + _glyphMetrics[c].xMin,
						destination.y + lineNumber*_lineSkip,
						metrics.width,
						_lineSkip};

			error |= (SDL_RenderCopy(renderer,
						_texture.getSDLTexture(),
						&source,
						&glyphDest) != 0);

			currentAdvance += metrics.advance;
		}
	}

	if(error)
	{
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"BitmapFont::renderText: "
			"Some SDL call returned error '%s'",
			SDL_GetError());
	}
}

void BitmapFont::renderDebug(SDL_Renderer * renderer,
				int const xDest,
				int const yDest)
{
	SDL_Rect dest{xDest, yDest, _texture.getWidth(), _texture.getHeight()};
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderCopy(renderer, _texture.getSDLTexture(), nullptr, &dest);

	SDL_SetRenderDrawColor(renderer, 255, 69, 0, 255);
	for(auto rect : _clips)
	{
		rect.x += xDest;
		rect.y += yDest;
		SDL_RenderDrawRect(renderer, &rect);
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