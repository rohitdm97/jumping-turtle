#include "Engine.h"
#include "debug.h"

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

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

		lightSourceShader = render::ShaderLoader("light_source").Create();
		restShader = render::ShaderLoader("light").Create();

		this-> model = comp::ModelLoader().Load("..\\assets\\porsche_911_930_turbo_1975", "scene.gltf");
		//this->model = comp::createModel();
		model.Attach(restShader);
		// from old captured camera settings
		camera = Camera(glm::vec3(1.5525, 4.472, 6.04472), glm::vec3(0, 1, 0), -104.6f, -29.0f);
		this->light = render::Light(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f));
		this->light.Attach(lightSourceShader);
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
		case engine::DEV_CAPTURE_CAMERA:
			LOG(DEBUG) << "camera: " << camera << "\n";
			break;
		case engine::ACTION_LAST:
			throw std::invalid_argument("invalid action, not supported");
		default:
			break;
		}
	}

	void Engine::Update() {
		glm::vec3 orange(209 / 255.0f, 68 / 255.0f, 21 / 255.0f);
		glm::vec3 blue(22 / 255.0f, 127 / 255.0f, 175 / 255.0f);
		float t = (float)sin(glfwGetTime() * 0.1f);
		light.SetColor((orange - blue) * t + blue);
		light.Rotate(glm::vec3(0, 1, 0), 0.1f);
		float a = (float)glfwGetTime() * 0.1f;
		glm::mat4 r = glm::rotate(glm::mat4(1.0), a, glm::vec3(-1,+1,+1));
		glm::vec4 p = r * glm::vec4(1.0, 1.0, 1.0, 1.0);
		light.SetPosition(glm::vec3(p.x,p.y,p.z));
	}

	void Engine::Render() {
		GCE

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		{
			auto& shader = restShader;
			shader.Activate();
			shader.Uniforms().SetCamera("camera", camera, window->AspectRatio());
			shader.Uniforms().SetLight("light", light);

			model.Render(shader);
		}

		{
			auto& shader = lightSourceShader;
			shader.Activate();
			shader.Uniforms(true).SetCamera("camera", camera, window->AspectRatio());
			shader.Uniforms(true).SetLight("light", light);
			light.Render(shader);
		}

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
		lastX = (int) xpos;
		lastY = (int) ypos;
		LOG(TRACE) << "cursor diff dx = " << dx << " dy = " << dy << "\n";
		camera.ProcessMouseMovement((float) dx, (float) dy);
	}

	void Engine::Scroll_callback(double, double yoffset) {
		LOG(TRACE) << "mouse was scrolled " << yoffset << "\n";
		camera.ProcessMouseScroll((float) yoffset);
	}

	Engine::~Engine() {
		LOG(TRACE) << "Engine is destroyed\n";
	}
}
