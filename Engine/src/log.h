#pragma once
#include <iostream>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

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
			bool isActive() const;
		public:
			Logger(Level);
			Logger& operator<<(int);
			Logger& operator<<(float);
			Logger& operator<<(double);
			Logger& operator<<(Level);
			Logger& operator<<(char);
			Logger& operator<<(bool);
			Logger& operator<<(const char*);
			Logger& operator<<(std::string);
			Logger& operator<<(const glm::vec3&);
			Logger& operator<<(const glm::vec4&);
		};

		Logger& _logging_maps_(Level level, const char* file, int line);
	}
}

#include <string.h>
#ifdef _WIN32
#define LIMITER_CHAR '\\'
#define DELIMITER_CHAR '\\'
#define DELIMITER_STR "\\"
#else
#define LIMITER_CHAR '/'
#define DELIMITER_CHAR '/'
#define DELIMITER_STR "/"
#endif // _WIN32

#define __FILENAME__ (strrchr(__FILE__, DELIMITER_CHAR) ? strrchr(__FILE__, DELIMITER_CHAR) + 1 : __FILE__)

#define LOG(level) util::logging::_logging_maps_(util::logging::Level::level, __FILENAME__, __LINE__)
