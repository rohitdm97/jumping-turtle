#pragma once

namespace engine {
	namespace fns {
		double GetSystemTime();
		double GetGlfwTime();
	}

	class Engine;
	class GameLoop {
		typedef double (*CurrentTimeFn)();
	private:
		CurrentTimeFn timeFn;
	public:
		GameLoop();
		GameLoop(CurrentTimeFn);

		void Start(Engine& engine);
	};
}
