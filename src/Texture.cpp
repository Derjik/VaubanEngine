#include <VBN/Surface.hpp>
#include <VBN/Texture.hpp>
#include <VBN/Logging.hpp>
#include <VBN/Exceptions.hpp>

Texture::Texture(SDL_Texture * rawTexture) :
	_rawTexture(rawTexture, &SDL_DestroyTexture),
	_pixelFormat(0),
	_access(0),
	_width(0),
	_height(0)
{
	if(rawTexture == nullptr)
		THROW(Exception, "Received nullptr 'rawTexture'");

	SDL_QueryTexture(_rawTexture.get(),
			&_pixelFormat,
			&_access,
			&_width,
			&_height);
}

Texture::Texture(Texture && other) :
	_rawTexture(std::move(other._rawTexture)),
	_clips(std::move(other._clips)),
	_pixelFormat(std::move(other._pixelFormat)),
	_access(std::move(other._access)),
	_width(std::move(other._width)),
	_height(std::move(other._height))
{}

Texture & Texture::operator=(Texture && other)
{
	this->_rawTexture = std::move(other._rawTexture);
	this->_clips = std::move(other._clips);
	this->_pixelFormat = std::move(other._pixelFormat);
	this->_access = std::move(other._access);
	this->_width = std::move(other._width);
	this->_height = std::move(other._height);

	return (*this);
}

SDL_Texture * Texture::getSDLTexture(void)
{
	return _rawTexture.get();
}

int Texture::getWidth(void) const
{
	return _width;
}

int Texture::getHeight(void) const
{
	return _height;
}

int Texture::getAccess(void) const
{
	return _access;
}

Uint32 Texture::getPixelFormat(void) const
{
	return _pixelFormat;
}

void Texture::setColorAlphaMod(SDL_Color const & color)
{
	if(SDL_SetTextureColorMod(_rawTexture.get(),
				color.r,
				color.g,
				color.b))
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Failed to set color and alpha : SDL error '%s'",
			SDL_GetError());
}

SDL_Color Texture::getColorAlphaMod(void) const
{
	SDL_Color rgba{0,0,0,0};

	if(SDL_GetTextureColorMod(_rawTexture.get(),
		&rgba.r,
		&rgba.g,
		&rgba.b))
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Failed to get color mod : SDL erorr '%s'",
			SDL_GetError());

	if(SDL_GetTextureAlphaMod(_rawTexture.get(),
		&rgba.a))
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Failed to get alpha mod : SDL error '%s'",
			SDL_GetError());

	return rgba;
}

Texture Texture::fromText(
	std::shared_ptr<TrueTypeFontManager> ttfManager,
	SDL_Renderer * renderer,
	std::string const & text,
	std::string const & textFontName,
	int const textSize,
	SDL_Color const & textColor)
{
	if (!ttfManager)
		THROW(Exception, "Received nullptr 'ttfManager'");
	if (renderer == nullptr)
		THROW(Exception, "Received nullptr 'renderer'");
	if (textFontName.empty())
		THROW(Exception, "Received empty 'textFontName'");
	if (textSize <= 0)
		THROW(Exception, "Received textSize <= 0");

	Surface textSurface(Surface::fromText(ttfManager, text, textFontName, textSize, textColor));
	return Texture::fromSurface(renderer, textSurface);
}

Texture Texture::fromSurface(
	SDL_Renderer * renderer,
	Surface & surface)
{
	if (renderer == nullptr)
		THROW(Exception, "Received nullptr 'renderer'");

	SDL_Texture * rawTexture =
		SDL_CreateTextureFromSurface(renderer, surface.getSurface());

	if (rawTexture == nullptr)
		THROW(Exception,
			"Cannot instantiate SDL_Texture : SDL error '%s'",
			SDL_GetError());

	return Texture(rawTexture);
}

Texture Texture::fromScratch(
	SDL_Renderer * renderer,
	Uint32 const format,
	int const access,
	int const width,
	int const height)
{
	if (renderer == nullptr)
		THROW(Exception, "Received nullptr 'renderer'");
	if (width <= 0)
		THROW(Exception, "Received 'width' <= 0");
	if (height <= 0)
		THROW(Exception, "Received 'height' <= 0");

	SDL_Texture * rawTexture = SDL_CreateTexture(
		renderer,
		format,
		access,
		width,
		height);

	if(rawTexture == nullptr)
		THROW(Exception,
			"Cannot instantiate SDL_Texture : SDL error '%s'",
			SDL_GetError());

	return Texture(rawTexture);
}

void Texture::addClip(std::string const & name,
	SDL_Rect const & clip)
{
	if (name.empty())
		THROW(Exception, "Received empty 'name'");
	if(_clips.find(name) != _clips.end())
		THROW(Exception,
			"Cannot override existing clip '%s'",
			name);

	_clips.emplace(make_pair(
		name,
		std::unique_ptr<SDL_Rect>(
			new SDL_Rect{clip.x, clip.y, clip.w, clip.h})));
}

SDL_Rect * Texture::getClip(std::string const & name)
{
	if (name.empty())
		THROW(Exception, "Received empty 'name'");

	auto clipIterator = _clips.find(name);

	if (clipIterator == _clips.end())
		THROW(Exception, "Cannot find clip '%s'", name);

	return _clips.at(name).get();
}