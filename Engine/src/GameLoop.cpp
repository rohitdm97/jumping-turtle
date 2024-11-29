
#include "GameLoop.h"

#include "Engine.h"

namespace engine {
	void GameLoop::Start(Engine& engine) {
		auto* window = engine.Window().Ref();
		while (!glfwWindowShouldClose(window)) {
			engine.KeyMap().ProcessInput(engine, window);

			engine.Update();

			engine.Render();

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
}
