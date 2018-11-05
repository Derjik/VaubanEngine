#ifndef LOGGING_HPP_INCLUDED
#define LOGGING_HPP_INCLUDED

#include <SDL2/SDL_log.h>

#ifdef _MSC_VER

#define CRITICAL(category, format, ...) \
		SDL_LogCritical(category, "%s:%d: " format, __FUNCTION__, __LINE__, __VA_ARGS__)
#define ERROR(category, format, ...) \
		SDL_LogError(category, "%s:%d: " format, __FUNCTION__, __LINE__, __VA_ARGS__)
#define WARNING(category, format, ...) \
		SDL_LogWarn(category, "%s:%d: " format, __FUNCTION__, __LINE__, __VA_ARGS__)
#define INFO(category, format, ...) \
		SDL_LogInfo(category, "%s:%d: " format, __FUNCTION__, __LINE__, __VA_ARGS__)
#define DEBUG(category, format, ...) \
		SDL_LogDebug(category, "%s:%d: " format, __FUNCTION__, __LINE__, __VA_ARGS__)
#define VERBOSE(category, format, ...) \
		SDL_LogVerbose(category, "%s:%d: " format, __FUNCTION__, __LINE__, __VA_ARGS__)

#else

#define CRITICAL(category, format, ...) \
		SDL_LogCritical(category, "%s:%d: " format, __func__, __LINE__, __VA_ARGS__)
#define ERROR(category, format, ...) \
		SDL_LogError(category, "%s:%d: " format, __func__, __LINE__, __VA_ARGS__)
#define WARNING(category, format, ...) \
		SDL_LogWarn(category, "%s:%d: " format, __func__, __LINE__, __VA_ARGS__)
#define INFO(category, format, ...) \
		SDL_LogInfo(category, "%s:%d: " format, __func__, __LINE__, __VA_ARGS__)
#define DEBUG(category, format, ...) \
		SDL_LogDebug(category, "%s:%d: " format, __func__, __LINE__, __VA_ARGS__)
#define VERBOSE(category, format, ...) \
		SDL_LogVerbose(category, "%s:%d: " format, __func__, __LINE__, __VA_ARGS__)

#endif // _MSC_VER

#endif // LOGGING_HPP_INCLUDED