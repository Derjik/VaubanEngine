#ifndef EXCEPTIONS_HPP_INCLUDED
#define EXCEPTIONS_HPP_INCLUDED

#include <stdexcept>
#include <string>

class Exception : public std::exception
{
	private:
		std::string _what;

	public:
		Exception(void);
		Exception(std::string what);
		Exception(char const * format, ...);
		Exception(Exception const &);
		Exception(Exception &&);
		Exception & operator = (Exception const &) = delete;
		Exception & operator = (Exception &&) = delete;
		~Exception(void);

		char const * what(void) const override;
};

#define THROW(exc, format, ...) \
		throw exc("%s:%d: " format, __func__, __LINE__, __VA_ARGS__)

#endif // EXCEPTIONS_HPP_INCLUDED
