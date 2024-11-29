#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace render {
	namespace windowcallback {
		void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		void window_size_callback(GLFWwindow* window, int width, int height);
		void window_content_scale_callback(GLFWwindow* window, float xscale, float yscale);
		void window_pos_callback(GLFWwindow* window, int xpos, int ypos);
		void mouse_callback(GLFWwindow* window, double, double);
		void scroll_callback(GLFWwindow* window, double, double);
	}
}

