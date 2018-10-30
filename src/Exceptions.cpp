#include <VBN/Exceptions.hpp>
#include <cstdarg>

Exception::Exception(void) : _what("")
{}

Exception::Exception(std::string what) : _what(what)
{}

Exception::Exception(char const * format, ...)
{
	va_list args;
	char buffer[1024];

	va_start(args, format);
	snprintf(buffer, sizeof(buffer), format, args);
	va_end(args);

	_what = buffer;
}

char const * Exception::what(void) const
{
	return _what.c_str();
}