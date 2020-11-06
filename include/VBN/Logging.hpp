#ifndef LOGGING_HPP_INCLUDED
#define LOGGING_HPP_INCLUDED

#include <SDL2/SDL_log.h>
#include <VBN/Exceptions.hpp>
#include <deque>
#include <string>

class Logger
{
	private:
		static unsigned int _maxBufferLines;
		static std::deque<std::string> _logBuffer;

	public:
		static void log(SDL_LogPriority level,
			int category,
			char const *format,
			...);
		static std::string lastLogs(void);
};

#ifdef _MSC_VER

#define EXCEPT(exception) \
		Logger::log(SDL_LOG_PRIORITY_CRITICAL, SDL_LOG_CATEGORY_ERROR, \
		"%s:%d: [!] EXCEPTION : <%s>", \
		__FUNCTION__, __LINE__, exception.what())

#define CRITICAL(category, format, ...) \
		Logger::log(SDL_LOG_PRIORITY_CRITICAL, category, "%s:%d: " format, __FUNCTION__, __LINE__, __VA_ARGS__)
#define ERROR(category, format, ...) \
		Logger::log(SDL_LOG_PRIORITY_ERROR, category, "%s:%d: " format, __FUNCTION__, __LINE__, __VA_ARGS__)
#define WARNING(category, format, ...) \
		Logger::log(SDL_LOG_PRIORITY_WARN, category, "%s:%d: " format, __FUNCTION__, __LINE__, __VA_ARGS__)
#define INFO(category, format, ...) \
		Logger::log(SDL_LOG_PRIORITY_INFO, category, "%s:%d: " format, __FUNCTION__, __LINE__, __VA_ARGS__)
#define DEBUG(category, format, ...) \
		Logger::log(SDL_LOG_PRIORITY_DEBUG, category, "%s:%d: " format, __FUNCTION__, __LINE__, __VA_ARGS__)
#define VERBOSE(category, format, ...) \
		Logger::log(SDL_LOG_PRIORITY_VERBOSE, category, "%s:%d: " format, __FUNCTION__, __LINE__, __VA_ARGS__)

#else

#define EXCEPT(exception) \
		Logger::log(SDL_LOG_PRIORITY_CRITICAL, SDL_LOG_CATEGORY_ERROR, \
		"%s:%d: [!] EXCEPTION : <%s>", \
		__func__, __LINE__, exception.what())

#define CRITICAL(category, format, ...) \
		Logger::log(SDL_LOG_PRIORITY_CRITICAL, category, "%s:%d: " format, __func__, __LINE__, __VA_ARGS__)
#define ERROR(category, format, ...) \
		Logger::log(SDL_LOG_PRIORITY_ERROR, category, "%s:%d: " format, __func__, __LINE__, __VA_ARGS__)
#define WARNING(category, format, ...) \
		Logger::log(SDL_LOG_PRIORITY_WARN, category, "%s:%d: " format, __func__, __LINE__, __VA_ARGS__)
#define INFO(category, format, ...) \
		Logger::log(SDL_LOG_PRIORITY_INFO, category, "%s:%d: " format, __func__, __LINE__, __VA_ARGS__)
#define DEBUG(category, format, ...) \
		Logger::log(SDL_LOG_PRIORITY_DEBUG, category, "%s:%d: " format, __func__, __LINE__, __VA_ARGS__)
#define VERBOSE(category, format, ...) \
		Logger::log(SDL_LOG_PRIORITY_VERBOSE, category, "%s:%d: " format, __func__, __LINE__, __VA_ARGS__)

#endif // _MSC_VER

#endif // LOGGING_HPP_INCLUDED
