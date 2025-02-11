#include "Log.h"

#include <map>



namespace util {
    namespace logging {

        extern Level _global_level_;
        extern std::map<Level, Logger*> _loggers_;

        std::ostream& operator<<(std::ostream& os, Level l) {
            switch (l) {
            case ALL:
                return os << "ALL";
            case TRACE:
                return os << "TRACE";
            case DEBUG:
                return os << "DEBUG";
            case INFO:
                return os << "INFO";
            case WARN:
                return os << "WARN";
            case ERROR:
                return os << "ERROR";
            case NONE:
                return os << "NONE";
            default:
                return os << "Level(" << ((int)l) << ")";
            }
        }

        Logger& _logging_maps_(Level level, const char* file, int line) {
            if (_loggers_.find(level) == _loggers_.end()) {
                _loggers_[level] = new Logger(level);
            }
            auto& l = *_loggers_[level];
            l << level << ":\t" << file << ":" << line << "\t\t";
            return l;
        }

        bool Logger::isActive() const {
            return _global_level_ <= this->level;
        }

        Logger::Logger(Level level) {
            this->level = level;
        }

        Logger& Logger::operator<<(int n) {
            if (!isActive()) {
                return *this;
            }
            std::cerr << n;
            return *this;
        }

        Logger& Logger::operator<<(float f) {
            if (!isActive()) {
                return *this;
            }
            std::cerr << f;
            return *this;
        }

        Logger& Logger::operator<<(double d) {
            if (!isActive()) {
                return *this;
            }
            std::cerr << d;
            return *this;
        }

        Logger& Logger::operator<<(Level l) {
            if (!isActive()) {
                return *this;
            }
            std::cerr << l;
            return *this;
        }

        Logger& Logger::operator<<(char n) {
            if (!isActive()) {
                return *this;
            }
            std::cerr << n;
            return *this;
        }

        Logger& Logger::operator<<(bool b) {
            if (!isActive()) {
                return *this;
            }
            std::cerr << (b ? "TRUE" : "FALSE");
            return *this;
        }

        Logger& Logger::operator<<(const char* arr) {
            if (!isActive()) {
                return *this;
            }
            std::cerr << arr;
            return *this;
        }

        Logger& Logger::operator<<(std::string str) {
            if (!isActive()) {
                return *this;
            }
            std::cerr << str;
            return *this;
        }

        Logger& Logger::operator<<(const glm::vec3& vec3) {
            if (!isActive()) {
                return *this;
            }
			std::cerr << "[" << vec3.x << ", " << vec3.y << ", " << vec3.z << "]";
            return *this;
        }

        Logger& Logger::operator<<(const glm::vec4& vec4) {
            if (!isActive()) {
                return *this;
            }
			std::cerr << "[" << vec4.x << ", " << vec4.y << ", " << vec4.z << ", " << vec4.w << "]";
            return *this;
        }

    }
}
