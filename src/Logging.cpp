#include <VBN/Logging.hpp>
#include <iostream>
#include <sstream>

std::deque<std::string> VBN_LogBuffer;

void VBN_Log(SDL_LogPriority priority, int category, char const * format, ...)
{
	va_list ap;
	va_start(ap, format);
	SDL_LogMessageV(category, priority, format, ap);
	char buffer[SDL_MAX_LOG_MESSAGE] = "";
	vsnprintf(buffer, SDL_MAX_LOG_MESSAGE - 1, format, ap);

	VBN_LogBuffer.push_front(buffer);
	while (VBN_LogBuffer.size() > VBN_LOG_BUFFER_LINES)
		VBN_LogBuffer.pop_back();

	va_end(ap);
}

std::string getLogAggregate(void)
{
	std::ostringstream s;

	std::deque<std::string>::reverse_iterator line;
	for (line = VBN_LogBuffer.rbegin() ; line != VBN_LogBuffer.rend() ; ++line)
		s << (*line) << '\n';

	return s.str();
}
