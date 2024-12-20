#include "Window.h"

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "log.h"
#include "Engine.h"
#include "WindowCallback.h"

void glDebugOutputCallback(
	GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* engine
) {
	printf("!!!EMERGENCY!!!\n");
	printf("message: id %d, %s\n", id, message);

	printf("source: ");
	switch (source) {
	case GL_DEBUG_SOURCE_API: printf("API"); break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: printf("WINDOW_SYSTEM"); break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: printf("SHADER_COMPILER"); break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: printf("THIRD_PARTY"); break;
	case GL_DEBUG_SOURCE_APPLICATION: printf("APPLICATION"); break;
	case GL_DEBUG_SOURCE_OTHER: printf("OTHER"); break;
	}
	printf("\n");

	printf("type: ");
	switch (type) {
	case GL_DEBUG_TYPE_ERROR: printf("ERROR"); break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: printf("DEPRECATED_BEHAVIOR"); break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: printf("UNDEFINED_BEHAVIOR"); break;
	case GL_DEBUG_TYPE_PORTABILITY: printf("PORTABILITY"); break;
	case GL_DEBUG_TYPE_PERFORMANCE: printf("PERFORMANCE"); break;
	case GL_DEBUG_TYPE_MARKER: printf("MARKER"); break;
	case GL_DEBUG_TYPE_PUSH_GROUP: printf("PUSH_GROUP"); break;
	case GL_DEBUG_TYPE_POP_GROUP: printf("POP_GROUP"); break;
	case GL_DEBUG_TYPE_OTHER: printf("OTHER"); break;
	}
	printf("\n");

	printf("severity: ");
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: printf("HIGH"); break;
	case GL_DEBUG_SEVERITY_MEDIUM: printf("MEDIUM"); break;
	case GL_DEBUG_SEVERITY_LOW: printf("LOW"); break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: printf("NOTIFICATION"); break;
	}
	printf("\n");
}

namespace render {
	void Window::Framebuffer_size_callback(int width, int height) {
		LOG(TRACE) << "Framebuffer_size_callback was called " << width << " " << height << "\n";
		glViewport(0, 0, width, height);
		this->width = width;
		this->height = height;
	}

	void Window::Window_size_callback(int width, int height) {
		LOG(TRACE) << "Window_size_callback was called " << width << " " << height << "\n";
	}

	void Window::Window_content_scale_callback(float xscale, float yscale) {
		LOG(TRACE) << "Window_content_scale_callback was called " << xscale << " " << yscale << "\n";
	}

	void Window::Window_pos_callback(int xpos, int ypos) {
		LOG(TRACE) << "Window_pos_callback was called " << xpos << " " << ypos << "\n";
	}

	Window::Window(engine::Engine* engine, int width, int height, std::string title) : engine(*engine), width(width), height(height), xpos(0), ypos(0) {
		auto titleChars = title.c_str();
		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);
		assert(monitors != nullptr && "failed to load the monitors");
		this->ref = glfwCreateWindow(width, height, titleChars, NULL, NULL);
		if (ref == NULL) {
			LOG(TRACE) << "Failed to create GLFW window\n";
			throw std::runtime_error("Failed to create a window");
		}
		glfwMakeContextCurrent(ref);
		glfwSetWindowUserPointer(ref, engine);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			LOG(ERROR) << "Failed to initialize GLAD\n";
			throw std::runtime_error("failed to initialize GLAD");
		}
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			LOG(ERROR) << "Failed to initialize GLAD\n";
			throw std::runtime_error("failed to initialize GLAD");
		}

		int contextFlags = 0;
		glGetIntegerv(GL_CONTEXT_FLAGS, &contextFlags);
		if (contextFlags & GL_CONTEXT_FLAG_DEBUG_BIT) {
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(glDebugOutputCallback, engine);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
		}

		glfwSetFramebufferSizeCallback(ref, windowcallback::framebuffer_size_callback);
		glfwSetWindowSizeCallback(ref, windowcallback::window_size_callback);
		glfwSetWindowContentScaleCallback(ref, windowcallback::window_content_scale_callback);
		glfwSetWindowPosCallback(ref, windowcallback::window_pos_callback);
		glfwSetCursorPosCallback(ref, windowcallback::mouse_callback);
		glfwSetScrollCallback(ref, windowcallback::scroll_callback);

		glfwSetInputMode(ref, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	GLFWwindow* Window::Ref() const {
		return ref;
	}
	float Window::AspectRatio() const {
		return width / ((float)height);
	}
}
