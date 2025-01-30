
#include "GameLoop.h"

#include "Engine.h"

#define MS_PER_UPDATE 1/60.

namespace engine {
	GameLoop::GameLoop() : GameLoop(fns::GetSystemTime) {
	}
	GameLoop::GameLoop(CurrentTimeFn fn) : timeFn(fn) {
	}
	void GameLoop::Start(Engine& engine) {
		auto* window = engine.Window().Ref();
		engine.Update();
		double lastTime = timeFn();
		double lag = 0;
		double lastFPSRecorded = lastTime;
		int upsCounter = 0, fpsCounter = 0;
		int lastUPS = 0, lastFPS = 0;
		while (!glfwWindowShouldClose(window)) {
			const double currTime = timeFn();
			const double elapsed = currTime - lastTime;
			lastTime = currTime;
			lag += elapsed;

			engine.GetKeyMap().ProcessInput(engine, window);

			while (lag > MS_PER_UPDATE) {
				upsCounter++;
				engine.Update();
				lag -= MS_PER_UPDATE;
			}

			fpsCounter++;
			engine.Render(lag / MS_PER_UPDATE);

			glfwSwapBuffers(window);
			glfwPollEvents();

			if (currTime - lastFPSRecorded > 1) {
				lastFPSRecorded = currTime;
				lastUPS = upsCounter;
				lastFPS = fpsCounter;
				upsCounter = 0;
				fpsCounter = 0;
#ifdef PRINT_FPS
				LOG(DEBUG) << "updates: " << lastUPS << ", frames: " << lastFPS << "\n";
#endif // PRINT_FPS
			}
		}
	}
}

#include <chrono>

double engine::fns::GetSystemTime() {
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	return ms.count();
}

#include <glad/glad.h>

double engine::fns::GetGlfwTime() {
	return glfwGetTime();
}
