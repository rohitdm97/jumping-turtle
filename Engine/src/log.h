#pragma once
#include <iostream>

namespace util {
	namespace logging {
		enum Level {
			ALL = 0,
			TRACE = 100,
			DEBUG = 200,
			INFO = 300,
			WARN = 400,
			ERROR = 500,
			NONE = ~0U >> 1,
		};
		std::ostream& operator<<(std::ostream& os, Level l);

		class Logger {
		private:
			Level level;
			bool isActive();
		public:
			Logger(Level);
			Logger& operator<<(int);
			Logger& operator<<(Level);
			Logger& operator<<(char);
			Logger& operator<<(bool);
			Logger& operator<<(const char*);
		};

		Logger& _logging_maps_(Level level, const char* file, int line);
	}
}

#include <string.h>
#ifdef _WIN32
#define LIMITER_CHAR '\\'
#else
#define LIMITER_CHAR '/'
#endif // _WIN32

#define __FILENAME__ (strrchr(__FILE__, LIMITER_CHAR) ? strrchr(__FILE__, LIMITER_CHAR) + 1 : __FILE__)

#define LOG(level) util::logging::_logging_maps_(util::logging::Level::level, __FILENAME__, __LINE__)
