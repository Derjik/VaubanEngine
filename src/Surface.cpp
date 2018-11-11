#include <SDL2/SDL_image.h>
#include <VBN/TrueTypeFontManager.hpp>
#include <VBN/Surface.hpp>
#include <VBN/Logging.hpp>
#include <VBN/Exceptions.hpp>


Surface::Surface(SDL_Surface * rawSurface) : _rawSurface(rawSurface, &SDL_FreeSurface)
{
	if (rawSurface == nullptr)
		THROW(Exception, "Received nullptr 'rawSurface'");
}

Surface::Surface(Surface && other) : _rawSurface(std::move(other._rawSurface))
{}

Surface & Surface::operator=(Surface && other)
{
	this->_rawSurface = std::move(other._rawSurface);
	return (*this);
}

SDL_Surface * Surface::getSurface(void)
{
	return _rawSurface.get();
}

Surface Surface::fromText(
			std::shared_ptr<TrueTypeFontManager> ttfManager,
			std::string const & text,
			std::string const & fontName,
			int const size,
			SDL_Color const & color)
{
	if (!ttfManager)
		THROW(Exception, "Received nullptr 'ttfManager'");
	if (fontName.empty())
		THROW(Exception, "Received empty 'fontName'");
	if (size <= 0)
		THROW(Exception, "Received 'size' <= 0");

	return Surface(ttfManager->getFont(fontName, size)
					.renderSolid(text, color));
}

Surface Surface::fromImage(std::string const & path)
{
	if (path.empty())
		THROW(Exception, "Received empty 'path'");

	SDL_Surface * surface(IMG_Load(path.c_str()));

	if(surface == nullptr)
		THROW(Exception,
			"Cannot instantiate SDL_Surface : IMG error '%s'",
			IMG_GetError());

	return Surface(surface);
}

Surface Surface::fromScratch(int const width,
			int const height,
			int const depth)
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	Uint32 rMask = 0xFF000000;
	Uint32 gMask = 0x00FF0000;
	Uint32 bMask = 0x0000FF00;
	Uint32 aMask = 0x000000FF;
#else
	Uint32 rMask = 0x000000FF;
	Uint32 gMask = 0x0000FF00;
	Uint32 bMask = 0x00FF0000;
	Uint32 aMask = 0xFF000000;
#endif

	SDL_Surface * surface(SDL_CreateRGBSurface(0, width, height, depth,
						rMask, gMask, bMask, aMask));

	if(surface == nullptr)
		THROW(Exception,
			"Cannot instantiate SDL_Surface : SDL error '%s'",
			SDL_GetError());

	Uint32 colorKey(SDL_MapRGBA(surface->format, 0, 0, 0, 0));

	if (SDL_SetColorKey(surface, SDL_TRUE, colorKey))
		THROW(Exception,
			"Failed to set colorkey : SDL error '%s'",
			SDL_GetError());

	return Surface(surface);
}
