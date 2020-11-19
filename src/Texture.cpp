#include <SDL2/SDL_render.h>
#include <VBN/Surface.hpp>
#include <VBN/Texture.hpp>
#include <VBN/Logging.hpp>
#include <VBN/Exceptions.hpp>

/*!
 * The main constructor for this class is private, external callers should use
 * the "from...()" factories instead.
 *
 * @param	rawTexture	SDL_Texture raw pointer to encapsulate in the internal
 *						std::unique_ptr (assuming ownership)
 * @throws	Exception	nullptr passed for rawTexture parameter
 */
Texture::Texture(SDL_Texture * rawTexture) :
	_rawTexture(rawTexture, &SDL_DestroyTexture),
	_pixelFormat(0),
	_access(0),
	_width(0),
	_height(0)
{
	// Check input parameters
	if(rawTexture == nullptr)
		THROW(Exception, "Received nullptr 'rawTexture'");

	// Acquire SDL_Texture metrics
	SDL_QueryTexture(
		_rawTexture.get(),
		&_pixelFormat,
		&_access,
		&_width,
		&_height);

	// Add a special, "global" clip matching the whole surface
	_clips.emplace("", SDL_Rect{ 0, 0, _width, _height });

	// Log
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Build Texture %p (SDL_Texture %p)",
		this,
		_rawTexture.get());
}

Texture::Texture(Texture && other) :
	_rawTexture(std::move(other._rawTexture)),
	_clips(std::move(other._clips)),
	_pixelFormat(std::move(other._pixelFormat)),
	_access(std::move(other._access)),
	_width(std::move(other._width)),
	_height(std::move(other._height))
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Move Texture %p (SDL_Texture %p) into new Texture %p",
		&other,
		_rawTexture.get(),
		this);
}

Texture & Texture::operator=(Texture && other)
{
	//! @todo Leak check below
	this->_rawTexture = std::move(other._rawTexture);
	this->_clips = std::move(other._clips);
	this->_pixelFormat = std::move(other._pixelFormat);
	this->_access = std::move(other._access);
	this->_width = std::move(other._width);
	this->_height = std::move(other._height);

	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Move (assign) Texture %p (SDL_Texture %p) into Texture %p",
		&other,
		_rawTexture.get(),
		this);

	return (*this);
}

Texture::~Texture(void)
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Delete Texture %p (SDL_Texture %p)",
		this,
		_rawTexture.get());
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

/*!
 * @param	ttfManager		Shared reference on a valid TrueTypeFontManager from
 *							which to extract the TrueTypeFont to use
 * @param	renderer		Raw pointer to the SDL_Renderer to use
 * @param	text			Latin1-encoded string to print
 * @param	textFontName	Font name
 * @param	textSize		Font size
 * @param	textColor		Font color
 * @returns					A newly created Texture instance containing the
 *							text render
 * @throws	Exception		Invalid input parameters, Surface instantiation
 *							or conversion error
 */
Texture Texture::fromLatin1Text(
	std::shared_ptr<TrueTypeFontManager> ttfManager,
	SDL_Renderer * renderer,
	std::string const & text,
	std::string const & textFontName,
	int const textSize,
	SDL_Color const & textColor)
{
	// Check input parameters
	if (!ttfManager)
		THROW(Exception, "Received nullptr 'ttfManager'");
	if (renderer == nullptr)
		THROW(Exception, "Received nullptr 'renderer'");
	if (textFontName.empty())
		THROW(Exception, "Received empty 'textFontName'");
	if (textSize <= 0)
		THROW(Exception, "Received textSize <= 0");

	// Instantiate Text Surface
	Surface textSurface(
		Surface::fromLatin1Text(
			ttfManager,
			text,
			textFontName,
			textSize,
			textColor));

	// Attempt conversion to Texture
	return Texture::fromSurface(renderer, textSurface);
}

/*!
 * @param	ttfManager		Shared reference on a valid TrueTypeFontManager from
 *							which to extract the TrueTypeFont to use
 * @param	renderer		Raw pointer to the SDL_Renderer to use
 * @param	text			UTF-8-encoded string to print
 * @param	textFontName	Font name
 * @param	textSize		Font size
 * @param	textColor		Font color
 * @returns					A newly created Texture instance containing the
 *							text render
 * @throws	Exception		Invalid input parameters, Surface instantiation
 *							or conversion error
 */
Texture Texture::fromUTF8Text(
	std::shared_ptr<TrueTypeFontManager> ttfManager,
	SDL_Renderer * renderer,
	std::string const & text,
	std::string const & textFontName,
	int const textSize,
	SDL_Color const & textColor)
{
	// Check input parameters
	if (!ttfManager)
		THROW(Exception, "Received nullptr 'ttfManager'");
	if (renderer == nullptr)
		THROW(Exception, "Received nullptr 'renderer'");
	if (textFontName.empty())
		THROW(Exception, "Received empty 'textFontName'");
	if (textSize <= 0)
		THROW(Exception, "Received textSize <= 0");

	// Instantiate Text Surface
	Surface textSurface(
		Surface::fromUTF8Text(
			ttfManager,
			text,
			textFontName,
			textSize,
			textColor));

	// Attempt conversion to Texture
	return Texture::fromSurface(renderer, textSurface);
}

/*!
 * @param	renderer	Raw pointer to the SDL_Renderer to use
 * @param	surface		Reference to the Surface instance to convert
 * @returns				A newly created Texture built from the input Surface
 *						contents
 * @throws	Exception	Invalid input parameters or SDL_Surface to SDL_Texture
 *						conversion error
 */
Texture Texture::fromSurface(
	SDL_Renderer * renderer,
	Surface & surface)
{
	// Check input parameters
	if (renderer == nullptr)
		THROW(Exception, "Received nullptr 'renderer'");

	// Attempt SDL_Surface to SDL_Texture conversion
	SDL_Texture * rawTexture =
		SDL_CreateTextureFromSurface(renderer, surface.getSurface());

	// Check for failure
	if (rawTexture == nullptr)
		THROW(Exception,
			"Cannot instantiate SDL_Texture : SDL error '%s'",
			SDL_GetError());

	// Return encapsulated Texture
	return Texture(rawTexture);
}

/*!
 * @param	renderer	Raw pointer to the SDL_Renderer to use
 * @param	format		Pixel format
 * @param	access		Access mode
 * @param	width		Width
 * @param	height		Height
 * @returns				A blank Texture created from input parameters
 * @throws				Invalid input parameters or SDL error
 */
Texture Texture::fromScratch(
	SDL_Renderer * renderer,
	Uint32 const format,
	int const access,
	int const width,
	int const height)
{
	// Check input parameters
	if (renderer == nullptr)
		THROW(Exception, "Received nullptr 'renderer'");
	if (width <= 0)
		THROW(Exception, "Received 'width' <= 0");
	if (height <= 0)
		THROW(Exception, "Received 'height' <= 0");

	// Attempt SDL_Texture creation from scratch
	SDL_Texture * rawTexture = SDL_CreateTexture(
		renderer,
		format,
		access,
		width,
		height);

	// Check for failure
	if(rawTexture == nullptr)
		THROW(Exception,
			"Cannot instantiate SDL_Texture : SDL error '%s'",
			SDL_GetError());

	// Return encapsulated Texture
	return Texture(rawTexture);
}

/*!
 * @param	name		Human-readable name to associate with the clipping rectangle
 * @param	clip		SDL_Rect representing the clipping area
 * @throws	Exception	Invalid input parameters
 */
void Texture::addClip(
	std::string const & name,
	SDL_Rect const & clip)
{
	// Check input parameters
	if (name.empty())
		THROW(Exception, "Received empty 'name'");
	if(_clips.find(name) != _clips.end())
		THROW(Exception,
			"Cannot override existing clip '%s'",
			name);

	// Store
	_clips.emplace(name, clip);
}

/*!
 * @param	name	Human-readable name of the wanted clip
 * @returns			Raw pointer to the SDL_Rect clip if it exists, nullptr
 *					otherwise
 */
SDL_Rect * Texture::getClip(std::string const & name)
{
	// Clip lookup
	auto clipIterator = _clips.find(name);
	if (clipIterator == _clips.end()) /* Miss */
		return nullptr;
	else
		return (&clipIterator->second); /* Hit */
}