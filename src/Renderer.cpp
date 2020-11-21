#include <VBN/Renderer.hpp>
#include <VBN/Surface.hpp>
#include <VBN/Logging.hpp>
#include <VBN/Exceptions.hpp>
#include <VBN/Introspection.hpp>

/*!
 * @param	window		Raw pointer to the SDL_Window for which the Renderer is
 *						instantiated
 * @param	flags		SDL_CreateRenderer() flags
 * @param	ttfManager	Shared reference to a TrueTypeFontManager to associate
 *						to the Renderer
 * @throws	Exception	Invalid input parameters or SDL call error
 *
 * @todo	Check default blending mode pertinence
 * @todo	Try Introspection cleanup
 */
Renderer::Renderer(
	SDL_Window * window,
	Uint32 flags,
	std::shared_ptr<TrueTypeFontManager> ttfManager) :
	_renderer(nullptr, &SDL_DestroyRenderer),
	_bitmapFontManager(nullptr),
	_trueTypeFontManager(ttfManager)
{
	// Check input parameters
	if (!window)
		THROW(Exception, "Received nullptr 'window'");
	if (!ttfManager)
		THROW(Exception, "Received nullptr 'ttfManager'");

	// Attempt renderer instantiation
	_renderer = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>(
		SDL_CreateRenderer(window, -1, flags),
		SDL_DestroyRenderer);
	// Check for errors
	if(_renderer == nullptr)
		THROW(Exception,
			"Cannot instantiate SDL_Renderer : SDL error '%s'",
			SDL_GetError());

	// Try instantiating a BitmapFontManager for the renderer
	_bitmapFontManager = std::unique_ptr<BitmapFontManager>(
		new BitmapFontManager(ttfManager, _renderer.get()));
	// Check for errors
	if(_bitmapFontManager == nullptr)
		THROW(Exception, "Cannot instantiate BitmapFontManager");

	// Set default blending mode (blend)
	setBlendMode(SDL_BLENDMODE_BLEND);

	// Give information on the newly instantiated renderer to the Hardware
	// Introspection module
	Introspection::insertRendererInfo(_renderer.get());

	// Log
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Build Renderer %p (SDL_Renderer %p)",
		this,
		_renderer.get());
}

Renderer::~Renderer(void)
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Delete Renderer %p (SDL_Renderer %p)",
		this,
		_renderer.get());
}

/*!
 * @param	textureName		Name to give to the newly created Texture in storage
 * @param	fontName		Font to use
 * @param	text			Latin1-encoded text to print on the texture
 * @param	size			Text size
 * @param	color			Text color
 * @throws	Exception		Invalid input parameters or SDL/TTF call error
 *
 * @todo	Unify API with other text-related methods
 */
void Renderer::addLatin1TextTexture(
	std::string const & textureName,
	std::string const & fontName,
	std::string const & text,
	int const size,
	SDL_Color const & color)
{
	// Check input parameters
	if(_textures.find(textureName) != _textures.end())
		THROW(Exception,
			"Cannot override existing texture '%s'",
			textureName);
	if (fontName.empty())
		THROW(Exception, "Received empty 'fontName'");
	if (size <= 0)
		THROW(Exception, "Received 'size' <= 0");

	// Attempt text rendering & storage into the Textures map
	_textures.emplace(
		make_pair(textureName,
			Texture::fromLatin1Text(
				_trueTypeFontManager,
				_renderer.get(),
				text,
				fontName,
				size,
				color)));
}

/*!
 * @param	textureName		Name to give to the newly created Texture in storage
 * @param	fontName		Font to use
 * @param	text			UTF-8-encoded text to print on the texture
 * @param	size			Text size
 * @param	color			Text color
 * @throws	Exception		Invalid input parameters or SDL/TTF call error
 *
 * @todo	Unify API with other text-related methods
 */
void Renderer::addUTF8TextTexture(
	std::string const & textureName,
		std::string const & fontName,
		std::string const & text,
		int const size,
		SDL_Color const & color)
{
	// Check input parameters
	if(_textures.find(textureName) != _textures.end())
		THROW(Exception,
			"Cannot override existing texture '%s'",
			textureName);
	if (fontName.empty())
		THROW(Exception, "Received empty 'fontName'");
	if (size <= 0)
		THROW(Exception, "Received 'size' <= 0");

	// Attempt text rendering & storage into the Textures map
	_textures.emplace(
		make_pair(textureName,
			Texture::fromUTF8Text(
				_trueTypeFontManager,
				_renderer.get(),
				text,
				fontName,
				size,
				color)));
}

/*!
 * @param	textureName		Name to give to the newly created Texture in the
 *							Renderer's internal storage
 * @param	path			Path to image file
 * @throws	Exception		Invalid input parameters or SDL call error
 */
void Renderer::addImageTexture(
	std::string const & textureName,
	std::string const & path)
{
	// Check input parameters
	if (_textures.find(textureName) != _textures.end())
		THROW(Exception,
			"Cannot override existing texture '%s'",
			textureName);
	if (path.empty())
		THROW(Exception, "Received empty 'path'");

	// Instantiate Surface from image & convert into Texture (may throw)
	Surface image(Surface::fromImage(path));

	// Store Texture into internal map
	_textures.emplace(
		make_pair(textureName,
			Texture::fromSurface(_renderer.get(), image)));
}

/*!
 * @param	name	Name of the Texture to query
 * @returns			The appropriate Texture for the input name, nullptr if not
 *					found
 */
Texture * Renderer::getTexture(std::string const & name)
{
	// Lookup
	if(_textures.find(name) == _textures.end()) /* Miss */
		return nullptr;
	else /* Hit */
		return (&_textures.at(name));
}

/*!
 * @param	text		Text to print (Latin1)
 * @param	fontName	Name of the font to use
 * @param	size		Text size
 * @param	color		Text color
 * @param	destination	Destination rectangle (will crop)
 * @throws	Exception	Invalid input parameters or SDL/TTF call error
 *
 * @todo	Unify API / handle UTF-8 BMF
 */
void Renderer::printText(
	std::string const & text,
	std::string const & fontName,
	int const size,
	SDL_Color const & color,
	SDL_Rect const & destination)
{
	// Check input parameters
	if (fontName.empty())
		THROW(Exception, "Received empty 'fontName'");
	if (size <= 0)
		THROW(Exception, "Received 'size' <= 0");

	// Try retrieving BitmapFont
	BitmapFont * font(_bitmapFontManager->getFont(fontName, size));
	if (font) /* Hit */
		// Render
		font->renderText(text, color, destination);
	else /* Miss */
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Cannot print dynamic text : missing font '%s' size '%d'",
			fontName.c_str(),
			size);
}

/*!
 * @param	fontName		Name of the font to use
 * @param	size			Debug text size
 * @param	xDest			X destination (top left pixel)
 * @param	yDest			Y destination (top left pixel)
 * @throws	Exception		Invalid input parameters or SDL/TTF call error
 *
 * @todo	Remove, at some point
 */
void Renderer::printDebugText(
	std::string const & fontName,
	int const size,
	int const xDest,
	int const yDest)
{
	BitmapFont * font = _bitmapFontManager->getFont(fontName, size);
	if (font)
		font->renderDebug(xDest, yDest);
	else
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Cannot print debug text : missing font '%s' size '%d'",
			fontName,
			size);
}

/*!
 * @param	red		Red component
 * @param	green	Green component
 * @param	blue	Blue component
 * @param	blue	Alpha component
 */
void Renderer::setDrawColor(
	Uint8 const red,
	Uint8 const green,
	Uint8 const blue,
	Uint8 const alpha)
{
	if (SDL_SetRenderDrawColor(_renderer.get(), red, green, blue, alpha))
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Cannot set renderer draw color : SDL error '%s'",
			SDL_GetError());
}

/*!
 * @param	color	SDL_Color to use
 */
void Renderer::setDrawColor(SDL_Color const & color)
{
	if (SDL_SetRenderDrawColor(_renderer.get(),
		color.r,
		color.g,
		color.b,
		color.a))
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Cannot set renderer draw color : SDL error '%s'",
			SDL_GetError());
}

void Renderer::setBlendMode(SDL_BlendMode const & blendMode)
{
	if (SDL_SetRenderDrawBlendMode(_renderer.get(), blendMode))
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Cannot set renderer blend mode : SDL eror '%s'",
			SDL_GetError());
}

void Renderer::setLogicalSize(int const w, int const h)
{
	if (SDL_RenderSetLogicalSize(_renderer.get(), w, h))
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Cannot set renderer logical size : SDL error '%s'",
			SDL_GetError());
}

void Renderer::setViewport(SDL_Rect const & viewport)
{
	if (SDL_RenderSetViewport(_renderer.get(), &viewport))
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Cannot set renderer viewport : SDL error '%s'",
			SDL_GetError());
}

void Renderer::resetViewport(void)
{
	if (SDL_RenderSetViewport(_renderer.get(), nullptr))
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Cannot reset renderer viewport : SDL error '%s'",
			SDL_GetError());
}

void Renderer::setScale(float const x, float const y)
{
	if (SDL_RenderSetScale(_renderer.get(), x, y))
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Cannot set renderer scale : SDL error '%s'",
			SDL_GetError());
}

void Renderer::clear(void)
{
	if(SDL_RenderClear(_renderer.get()))
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Cannot clear renderer : SDL error '%s'",
			SDL_GetError());
}

void Renderer::fill(void)
{
	if(SDL_RenderFillRect(_renderer.get(), nullptr))
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Cannot fill renderer canvas : SDL error '%s'",
			SDL_GetError());
}

void Renderer::fillRect(SDL_Rect const & rectangle)
{
	if(SDL_RenderFillRect(_renderer.get(), &rectangle))
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Cannot fill rectangle : SDL error '%s'",
			SDL_GetError());
}

void Renderer::drawRect(SDL_Rect const & rectangle)
{
	if(SDL_RenderDrawRect(_renderer.get(), &rectangle))
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Cannot draw rectangle : SDL error '%s'",
			SDL_GetError());
}

void Renderer::drawLine(
	int const x1,
	int const y1,
	int const x2,
	int const y2)
{
	if (SDL_RenderDrawLine(_renderer.get(), x1, y1, x2, y2))
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Cannot draw line : SDL error '%s'",
			SDL_GetError());
}

/*!
 * @param	textureName		Name of the Texture to copy on rendering space
 * @param	clipName		Name of the clipping rectangle to use ("" = entire
 *							Texture)
 * @param	destination		Destination rectangle for the rendering
 * @throws	Exception		Invalid input parameters or SDL call error
 *
 * @todo	Add 1:1 version
 */
void Renderer::copy(
	std::string const & textureName,
	std::string const & clipName,
	SDL_Rect const & destination)
{
	// Texture lookup
	auto textureIterator = _textures.find(textureName);
	if (textureIterator == _textures.end())
	{
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Cannot copy texture '%s' : not found in _textures",
			textureName.c_str());
		return;
	}

	// Clip lookup
	SDL_Rect * clip = textureIterator->second.getClip(clipName);
	if (!clip)
	{
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"No clip '%s' on texture '%s'",
			clipName.c_str(),
			textureName.c_str());
		return;
	}

	SDL_Texture * sdlTexture = textureIterator->second.getSDLTexture();

	// Rendering attempt
	if (SDL_RenderCopy(_renderer.get(), sdlTexture, clip, &destination))
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Cannot copy texture '%s' with clip '%s' : SDL error '%s'",
			textureName.c_str(),
			clipName.c_str(),
			SDL_GetError());
}

/*!
 * @param	textureName		Name of the Texture to copy on rendering space
 * @param	clipName		Name of the clipping rectangle to use ("" = entire
 *							Texture)
 * @param	destination		Destination rectangle for the rendering
 * @param	angle			Angle of rotation in degrees (0 = no rotation)
 * @param	center			Center of rotation
 * @param	flip			SDL flip flag
 * @throws	Exception		Invalid input parameters or SDL call error
 *
 * @todo	Add 1:1 version
 */
void Renderer::copyEx(
	std::string const & textureName,
	std::string const & clipName,
	SDL_Rect const & destination,
	double const angle,
	SDL_Point const & center,
	SDL_RendererFlip const & flip)
{
	// Texture lookup
	auto textureIterator = _textures.find(textureName);
	if (textureIterator == _textures.end())
	{
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Cannot copy texture '%s' : not found in _textures",
			textureName.c_str());
		return;
	}

	// Clip lookup
	SDL_Rect * clip = textureIterator->second.getClip(clipName);
	if (!clip)
	{
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"No clip '%s' on texture '%s'",
			clipName.c_str(),
			textureName.c_str());
		return;
	}

	SDL_Texture * sdlTexture = textureIterator->second.getSDLTexture();

	// Rendering attempt
	if (SDL_RenderCopyEx(_renderer.get(),
		sdlTexture, clip, &destination,
		angle, &center, flip))
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Cannot copy texture '%s' with clip '%s' : SDL error '%s'",
			textureName.c_str(),
			clipName.c_str(),
			SDL_GetError());
}

/*!
 * @todo	Handle errors
 */
void Renderer::present(void)
{
	SDL_RenderPresent(_renderer.get());
}