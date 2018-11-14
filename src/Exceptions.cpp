#include <VBN/Exceptions.hpp>
#include <VBN/Logging.hpp>
#include <cstdarg>

Exception::Exception(void) : _what("")
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Build Exception %p",
		this);
}

Exception::Exception(std::string what) : _what(what)
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Build Exception %p ('%s')",
		this,
		_what.c_str());
}

Exception::Exception(Exception const & other) : _what(other._what)
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Copy Exception %p ('%s') into new Exception %p",
		&other,
		_what.c_str(),
		this);
}

Exception::Exception(Exception && other) : _what(std::move(other._what))
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Move Exception %p ('%s') into new Exception %p",
		&other,
		_what.c_str(),
		this);
}

Exception::~Exception(void)
{
	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Delete Exception %p ('%s')",
		this,
		_what);
}

Exception::Exception(char const * format, ...)
{
	va_list args;
	char buffer[1024];

	va_start(args, format);
	snprintf(buffer, sizeof(buffer), format, args);
	va_end(args);

	_what = buffer;

	VERBOSE(SDL_LOG_CATEGORY_APPLICATION,
		"Build Exception %p ('%s')",
		this,
		_what.c_str());
}

char const * Exception::what(void) const
{
	return _what.c_str();
}