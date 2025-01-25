/**
*/
#include "log.h"
#include "OS.h"

#include "Game.h"
#include "Engine.h"

#include <string>
#include <sstream>
#include <map>

namespace util {
	namespace logging {
		Level _global_level_ = DEBUG;
		std::map<Level, Logger*> _loggers_;
	}
}

namespace engine {
	bool Engine::init = Engine::init || initGlfw();
}

int main(int argc, char** argv) {
	getCurrentDirectory();
	std::stringstream ss;
	ss << "[" << argc << "] ";
	for (int i = 0; i < argc; i++) {
		ss << argv[i] << " ";
	}
	LOG(INFO) << ss.str().c_str() << "\n";
	LOG(INFO) << "This is the game\n";
	{
		Game game;
		{
			engine::Engine engine(game);
			engine.Load();
			engine.Start();
		}
	}
	return 0;
}
