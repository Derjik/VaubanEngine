#include <SDL2/SDL_image.h>
#include <VBN/TrueTypeFontManager.hpp>
#include <VBN/Surface.hpp>


Surface::Surface(SDL_Surface * rawSurface) : _rawSurface(rawSurface)
{}

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
	TrueTypeFont & font = ttfManager->getFont(fontName, size);
	return Surface(font.renderSolid(text, color));
}

Surface Surface::fromImage(std::string const & path)
{
	SDL_Surface * surface(IMG_Load(path.c_str()));

	if(surface == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			"Surface::fromImage: IMG_Load returned error '%s'",
			IMG_GetError());
		throw std::string(
			"Surface::fromImage: IMG_Load returned error '"
			+ std::string(IMG_GetError()) + "'");
	}
	else
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
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			"Surface::fromScratch: SDL_CreateRGBSurface returned "
			"error '%s'", SDL_GetError());
		throw std::string(
			"Surface::fromScratch: SDL_CreateRGBSurface returned "
			"error '" + std::string(SDL_GetError()) + "'");
	}

	if (SDL_SetColorKey(surface,
		SDL_TRUE,
		SDL_MapRGBA(surface->format, 0, 0, 0, 0)))
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			"Surface::fromScratch: SDL_SetColorKey returned "
			"error '%s'", SDL_GetError());
		throw std::string(
			"Surface::fromScratch: SDL_SetColorKey returned "
			"error '" + std::string(SDL_GetError()) + "'");
	}

	return Surface(surface);
}

void SDLSurfaceDeleter::operator() (SDL_Surface * surface) const
{
	SDL_FreeSurface(surface);
}
