#include "Engine.h"
#include "debug.h"

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <string>
#include "log.h"

bool initGlfw() {
	LOG(TRACE) << "Initializing GLFW 3.3\n";
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef ENABLE_DEBUG_CONTEXT
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

	return true;
}

int lastX = 0;
int lastY = 0;
bool firstMouseEvent = true;

namespace engine {
	Engine::Engine(Game& game) : game(game) {
		LOG(TRACE) << "Engine is created for the game\n";
	}

	void Engine::Load() {
		this->window = std::make_unique<render::Window>(this, 800, 600, "this is title");
		GCE;

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		GCE;

		shader = render::ShaderLoader("camera").Create();
		//this-> model = comp::ModelLoader().Load("..\\assets\\porsche_911_930_turbo_1975", "scene.gltf");
		this->model = comp::createModel();
		model.Attach(shader);
		camera.SetPosition(glm::vec3(0.0f, 0.0f, +3.0f));
		// load scenes
		// load shaders
		// load textures
		GCE
	}

	void Engine::Start() {
		loop.Start(*this);
	}

	KeyMap& Engine::KeyMap() {
		return keyMap;
	}

	render::Window& Engine::Window() const {
		return *window;
	}

	void Engine::ProcessInput(Action a) {
		const double delta = 1.0 / 60.0;
		switch (a) {
		case engine::EXIT:
			glfwSetWindowShouldClose(Window().Ref(), true);
			break;
		case engine::MOVE_FORWARD:
			camera.ProcessKeyboard(camera::FORWARD, delta);
			break;
		case engine::MOVE_LEFT:
			camera.ProcessKeyboard(camera::LEFT, delta);
			break;
		case engine::MOVE_BACKWARD:
			camera.ProcessKeyboard(camera::BACKWARD, delta);
			break;
		case engine::MOVE_RIGHT:
			camera.ProcessKeyboard(camera::RIGHT, delta);
			break;
		case engine::MOVE_UP:
			break;
		case engine::MOVE_DOWN:
			break;
		case engine::ACTION_LAST:
			throw std::invalid_argument("invalid action, not supported");
		default:
			break;
		}
	}

	void Engine::Update() {
	}

	void Engine::Render() {
		GCE

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Activate();
		shader.SetMat4("CameraView", camera.GetViewMatrix());
		shader.SetMat4("CameraProjection", camera.GetProjectionMatrix(window->AspectRatio()));
		model.Render(shader);

		GCE
	}

	void Engine::Mouse_callback(double xpos, double ypos) {
		if (firstMouseEvent) {
			firstMouseEvent = false;
			lastX = (int) xpos;
			lastY = (int) ypos;
		}
		LOG(TRACE) << "cursor was moved x = " << xpos << " y = " << ypos << "\n";
		double dx = xpos - lastX;
		double dy = lastY - ypos;
		lastX = xpos;
		lastY = ypos;
		LOG(TRACE) << "cursor diff dx = " << dx << " dy = " << dy << "\n";
		camera.ProcessMouseMovement((float) dx, (float) dy);
	}

	void Engine::Scroll_callback(double xoffset, double yoffset) {
		LOG(TRACE) << "mouse was scrolled " << yoffset << "\n";
		camera.ProcessMouseScroll((float) yoffset);
	}

	Engine::~Engine() {
		LOG(TRACE) << "Engine is destroyed\n";
	}
}
