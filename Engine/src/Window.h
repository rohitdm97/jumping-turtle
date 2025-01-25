#pragma once
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <string>

namespace engine {
	class Engine;
}

namespace render {
	class Window {
	private:
		int width;
		int height;
		int xpos;
		int ypos;
		GLFWwindow* ref;
		engine::Engine& engine;
	public:
		Window(engine::Engine*, int width, int height, std::string title);
		GLFWwindow* Ref() const;
		float AspectRatio() const;
		int Width() const;
		int Height() const;

		void Framebuffer_size_callback(int width, int height);
		void Window_size_callback(int width, int height);
		void Window_content_scale_callback(float xscale, float yscale);
		void Window_pos_callback(int xpos, int ypos);
	};
}
