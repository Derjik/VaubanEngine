#include <VBN/Renderer.hpp>
#include <VBN/Logging.hpp>
#include <VBN/Exceptions.hpp>
#include <VBN/Introspection.hpp>

#define DEFAULT_TEXTURE "default"

Renderer::Renderer(SDL_Window * window,
	int index,
	Uint32 flags,
	std::shared_ptr<TrueTypeFontManager> ttfManager) :
	_renderer(nullptr, &SDL_DestroyRenderer),
	_bitmapFontManager(nullptr)
{
	INFO(SDL_LOG_CATEGORY_RENDER, "Build Renderer : "
		"Window 0x%08X - Index %d - flags 0x%08X - TTFManager 0x%08X",
		window, index, flags, ttfManager);

	if (!window)
		THROW(Exception, "Received nullptr 'window'");

	_renderer = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>(
		SDL_CreateRenderer(window, -1, flags),
		&SDL_DestroyRenderer);
	if(_renderer == nullptr)
		THROW(Exception,
			"Cannot instantiate SDL_Renderer : SDL error '%s'",
			SDL_GetError());

	_textures.emplace(std::make_pair(
		std::string(DEFAULT_TEXTURE),
		Texture::fromScratch(
			_renderer.get(),
			SDL_PIXELFORMAT_RGBA32,
			SDL_TEXTUREACCESS_STATIC,
			256, 256)));

	_bitmapFontManager = std::unique_ptr<BitmapFontManager>(
		new BitmapFontManager(ttfManager, _renderer.get()));
	if(_bitmapFontManager == nullptr)
		THROW(Exception, "Cannot instantiate BitmapFontManager");

	setBlendMode(SDL_BLENDMODE_BLEND);

	Introspection::insertRendererInfo(_renderer.get());
}

//SDL_Renderer * Renderer::getSDLRenderer(void)
//{
//	return _renderer.get();
//}

void Renderer::addTextTexture(std::string const & textureName,
			std::shared_ptr<TrueTypeFontManager> ttfManager,
			std::string const & fontName,
			std::string const & text,
			int const size,
			SDL_Color const & color)
{
	if(_textures.find(textureName) != _textures.end())
		THROW(Exception,
			"Cannot override existing texture '%s'",
			textureName);
	if (!ttfManager)
		THROW(Exception, "Received nullptr 'ttfManager'");
	if (fontName.empty())
		THROW(Exception, "Received empty 'fontName'");
	if (size <= 0)
		THROW(Exception, "Received 'size' <= 0");

	_textures.emplace(
		make_pair(textureName,
			Texture::fromText(ttfManager, _renderer.get(),
				text, fontName, size, color)));
}

void Renderer::addImageTexture(std::string const & textureName,
	std::string const & path)
{
	if (_textures.find(textureName) != _textures.end())
		THROW(Exception,
			"Cannot override existing texture '%s'",
			textureName);
	if (path.empty())
		THROW(Exception, "Received empty 'path'");

	Surface image(Surface::fromImage(path));

	_textures.emplace(
		make_pair(textureName,
			Texture::fromSurface(_renderer.get(), image)));
}

Texture * Renderer::getTexture(std::string const & name)
{
	if(_textures.find(name) == _textures.end())
		return nullptr;
	else
		return (&_textures.at(name));
}

void Renderer::printText(std::string const & text,
		std::string const & fontName,
		int const size,
		SDL_Color const & color,
		SDL_Rect const & destination)
{
	if (fontName.empty())
		THROW(Exception, "Received empty 'fontName'");
	if (size <= 0)
		THROW(Exception, "Received 'size' <= 0");

	BitmapFont & font = _bitmapFontManager->getFont(fontName, size);
	font.renderText(text, color, destination, _renderer.get());
}

void Renderer::printDebugText(std::string const & fontName,
	int const size,
	int const xDest,
	int const yDest)
{
	BitmapFont & font = _bitmapFontManager->getFont(fontName, size);
	font.renderDebug(_renderer.get(), xDest, yDest);
}

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

void Renderer::copy(
	std::string const & textureName,
	std::string const & clipName,
	SDL_Rect const & destination)
{
	auto textureIterator = _textures.find(textureName);
	if (textureIterator == _textures.end())
	{
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Cannot copy texture '%s' : not found in _textures",
			textureName.c_str());
		return;
	}

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

	if (SDL_RenderCopy(_renderer.get(), sdlTexture, clip, &destination))
		ERROR(SDL_LOG_CATEGORY_ERROR,
			"Cannot copy texture '%s' with clip '%s' : SDL error '%s'",
			textureName.c_str(),
			clipName.c_str(),
			SDL_GetError());
}

void Renderer::present(void)
{
	SDL_RenderPresent(_renderer.get());
}