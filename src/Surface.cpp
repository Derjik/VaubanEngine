#include <VBN/Surface.hpp>
#include <VBN/TrueTypeFontManager.hpp>
#include <VBN/Logging.hpp>
#include <VBN/Exceptions.hpp>
#include <SDL2/SDL_image.h>

/*!
 * The main constructor for this class is private, external callers should use
 * the "from...()" factories instead.
 *
 * @param	rawSurface	SDL_Surface raw pointer to encapsulate in the internal
 *						std::unique_ptr (assuming ownership)
 * @throws	Exception	nullptr passed for rawTexture parameter
 */
Surface::Surface(SDL_Surface * rawSurface) :
	_rawSurface(rawSurface, &SDL_FreeSurface)
{
	// Check input parameters
	if (rawSurface == nullptr)
		THROW(Exception, "Received nullptr 'rawSurface'");

	// Log
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Build Surface %p (SDL_Surface %p)",
		this,
		_rawSurface.get());
}

Surface::Surface(Surface && other) : _rawSurface(std::move(other._rawSurface))
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Move Surface %p (SDL_Surface %p) into new Surface %p",
		&other,
		_rawSurface.get(),
		this);
}

Surface::~Surface(void)
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Delete Surface %p (SDL_Surface %p)",
		this,
		_rawSurface.get());
}

SDL_Surface * Surface::getSurface(void)
{
	return _rawSurface.get();
}

/*!
 * @param	ttfManager		Shared reference to the TrueTypeFontManager to use
 * @param	text			Latin1-encoded string to print
 * @param	fontName		Name of the font to use
 * @param	size			Font size
 * @param	color			Font color
 * @returns					A newly created Surface with printed text
 * @throws	Exception		Invalid input parameters or TTF call error
 */
Surface Surface::fromLatin1Text(
			std::shared_ptr<TrueTypeFontManager> ttfManager,
			std::string const & text,
			std::string const & fontName,
			int const size,
			SDL_Color const & color)
{
	// Check input parameters
	if (!ttfManager)
		THROW(Exception, "Received nullptr 'ttfManager'");
	if (fontName.empty())
		THROW(Exception, "Received empty 'fontName'");
	if (size <= 0)
		THROW(Exception, "Received 'size' <= 0");

	// Try fetching font from TrueTypeFontManager
	TrueTypeFont * font(ttfManager->getFont(fontName, size));
	if (!font) /* No such font */
		THROW(Exception,
			"Cannot retrieve font '%s' size '%d'",
			fontName,
			size);

	// Try rendering input text into a raw SDL_Surface, then encapsulate
	// (may throw)
	return Surface(font->renderSolidLatin1(text, color));
}

/*!
 * @param	ttfManager		Shared reference to the TrueTypeFontManager to use
 * @param	text			UTF-8-encoded string to print
 * @param	fontName		Name of the font to use
 * @param	size			Font size
 * @param	color			Font color
 * @returns					A newly created Surface with printed text
 * @throws	Exception		Invalid input parameters or TTF call error
 */
Surface Surface::fromUTF8Text(
			std::shared_ptr<TrueTypeFontManager> ttfManager,
			std::string const & text,
			std::string const & fontName,
			int const size,
			SDL_Color const & color)
{
	// Check input parameters
	if (!ttfManager)
		THROW(Exception, "Received nullptr 'ttfManager'");
	if (fontName.empty())
		THROW(Exception, "Received empty 'fontName'");
	if (size <= 0)
		THROW(Exception, "Received 'size' <= 0");

	// Try fetching font from TrueTypeFontManager
	TrueTypeFont * font(ttfManager->getFont(fontName, size));
	if (!font) /* No such font */
		THROW(Exception,
			"Cannot retrieve font '%s' size '%d'",
			fontName,
			size);

	// Try rendering input text into a raw SDL_Surface, then encapsulate
	// (may throw)
	return Surface(font->renderSolidUTF8(text, color));
}

/*!
 * @param	path	Full or relative path to image file
 * @returns			A newly created Surface with the given image
 * @throws			Invalid input parameters or SDL IMG call error
 */
Surface Surface::fromImage(std::string const & path)
{
	// Check input parameters
	if (path.empty())
		THROW(Exception, "Received empty 'path'");

	// Try loading image in raw SDL_Surface
	SDL_Surface * surface(IMG_Load(path.c_str()));

	// Check for loading errors
	if(surface == nullptr)
		THROW(Exception,
			"Cannot instantiate SDL_Surface : IMG error '%s'",
			IMG_GetError());

	// Return encapsulated Surface
	return Surface(surface);
}

/*!
 * @param	width	Surface width
 * @param	height	Surface height
 * @param	depth	Surface color depth
 * @returns			Blank Surface
 * @throws			SDL call error
 */
Surface Surface::fromScratch(
	int const width,
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

	// Try creating blank surface from parameters
	SDL_Surface * surface(
		SDL_CreateRGBSurface(
			0,
			width,
			height,
			depth,
			rMask,
			gMask,
			bMask,
			aMask));

	// Check for creation error
	if(surface == nullptr)
		THROW(Exception,
			"Cannot instantiate SDL_Surface : SDL error '%s'",
			SDL_GetError());

	// Try setting alpha
	Uint32 colorKey(SDL_MapRGBA(surface->format, 0, 0, 0, 0));

	// Check for errors
	if (SDL_SetColorKey(surface, SDL_TRUE, colorKey))
		THROW(Exception,
			"Failed to set colorkey : SDL error '%s'",
			SDL_GetError());

	// Return encapsulated surface
	return Surface(surface);
}
