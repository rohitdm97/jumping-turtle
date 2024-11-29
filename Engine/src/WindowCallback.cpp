#include "WindowCallback.h"
#include "Engine.h"
#include "Window.h"

namespace render {
	namespace windowcallback {
		engine::Engine& GetEngine(GLFWwindow* window) {
			auto* engine = reinterpret_cast<engine::Engine*>(glfwGetWindowUserPointer(window));
			return *engine;
		}

		void SetEngine(GLFWwindow* window, engine::Engine* engine) {
			glfwSetWindowUserPointer(window, engine);
		}

		void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
			auto& engine = GetEngine(window);
			engine.Window().Framebuffer_size_callback(width, height);
		}

		void window_size_callback(GLFWwindow* window, int width, int height) {
			auto& engine = GetEngine(window);
			engine.Window().Window_size_callback(width, height);
		}

		void window_content_scale_callback(GLFWwindow* window, float xscale, float yscale) {
			auto& engine = GetEngine(window);
			engine.Window().Window_content_scale_callback(xscale, yscale);
		}

		void window_pos_callback(GLFWwindow* window, int xpos, int ypos) {
			auto& engine = GetEngine(window);
			engine.Window().Window_pos_callback(xpos, ypos);
		}

		void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
			auto& engine = GetEngine(window);
			engine.Mouse_callback(xpos, ypos);
		}

		void scroll_callback(GLFWwindow* window, double xpos, double ypos) {
			auto& engine = GetEngine(window);
			engine.Scroll_callback(xpos, ypos);
		}

	}
}

