#include "Engine.h"
#include "debug.h"

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include "log.h"

bool initGlfw() {
	LOG(TRACE) << "Initializing GLFW 4.5\n";
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
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
		UnsafeAlloc_ = {};
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

		lightSourceShader = render::ShaderLoader("light_caster.src").Create();
		boldShader = render::ShaderLoader("bold_color").Create();
		restShader = render::ShaderLoader("light_caster").Create();

		this->keyMap = engine::KeyMap(*this);
		this->spawner = std::unique_ptr<comp::Model>(comp::CreateParticleSpawner());
		this->scene = std::make_unique<scene::Scene>();
		flyCamera = Camera().WithPosition(glm::vec3(1.5525, 4.472, 6.04472)).WithLookAt(glm::vec3());
		topView = SideViewCamera(SideViewCamera::TOP);
		leftView = SideViewCamera(SideViewCamera::LEFT);
		frontView = SideViewCamera(SideViewCamera::FRONT);

		glm::vec3 orange(242/ 255.0f, 197 / 255.0f, 170 / 255.0f);
		this->light = render::Light(glm::vec3(1.0f, 1.0f, 1.0f), orange);

		restShader.Activate();
		restShader.Uniforms().SetInt("lights_length", 1);

		// load scenes
		// load shaders
		// load textures
		GCE
	}

	void Engine::Start() {
		loop.Start(*this);
	}

	KeyMap& Engine::GetKeyMap() {
		return keyMap;
	}

	render::Window& Engine::Window() const {
		return *window;
	}

	Camera& Engine::FlyCamera() {
		return flyCamera;
	}

	entities::System& Engine::EntitySystem() {
		return entitySystem;
	}

	const comp::Model& Engine::GetSpawner() const {
		return *spawner;
	}

	render::Light& Engine::Light() {
		return light;
	}

	void Engine::Update() {
		const glm::vec3 orange(209 / 255.0f, 68 / 255.0f, 21 / 255.0f);
		const glm::vec3 blue(22 / 255.0f, 127 / 255.0f, 175 / 255.0f);
		const float t = (float)sin(glfwGetTime() * 0.1f);
		//light.SetColor((orange - blue) * t + blue);
		//light.Rotate(glm::vec3(0, 1, 0), 0.1f);
		const float a = (float)glfwGetTime() * 0.1f;
		const glm::mat4 r = glm::rotate(glm::mat4(1.0), a, glm::vec3(-1,+1,+1));
		const glm::vec4 p = r * glm::vec4(1.0, 1.0, 1.0, 1.0);
		//light.SetPosition(glm::vec3(p.x,p.y,p.z));
	}

	void Engine::render(ViewPort vp, CameraMatrixProvider& camera, render::Shader& restShader_, render::Shader& lightSourceShader_) {
		glViewport(vp.x, vp.y, vp.width, vp.height);
		const float ar = vp.width / ((float) vp.height);

		{
			auto& shader = restShader_;
			shader.Activate();
			camera.SetUniforms("camera", shader.Uniforms(), ar);
			light.SetUniforms("lights[0]", shader.Uniforms());

			scene->Render(shader);
		}

		{
			auto& shader = lightSourceShader_;
			shader.Activate();
			camera.SetUniforms("camera", shader.Uniforms(true), ar);
			light.SetUniforms("light", shader.Uniforms(true));
			light.Render(shader);
		}
	}

	void Engine::Render(double) {
		GCE;

		glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#define SPLIT_VIEWPORT1
#ifdef SPLIT_VIEWPORT
		glViewport(0, 0, window->Width(), window->Height());
		const int w_2 = window->Width() / 2;
		const int h_2 = window->Height() / 2;
		for (unsigned int i = 0; i < 4; i++) {
			const unsigned int row = i / 2;
			const unsigned int col = i % 2;
			ViewPort vp = {
				.x = (int) col * w_2,
				.y = (int) row * h_2,
				.width = w_2,
				.height = h_2,
			};
			if (i == 1) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			switch (i) {
			case 0:
				render(vp, frontView, boldShader, boldShader);break;
			case 1:
				render(vp, flyCamera, restShader, lightSourceShader);break;
			case 2:
				render(vp, leftView, boldShader, boldShader);break;
			case 3:
				render(vp, topView, boldShader, boldShader);break;
			default:
				assert(false);
			}
		//	glViewport(, row * h_2, (col + 1) * w_2, (row + 1) * h_2);
#else
		glViewport(0, 0, window->Width(), window->Height());
		auto i = 1;
		const unsigned int row = i / 2;
		const unsigned int col = i % 2;
		ViewPort vp = {
			.x = 0,
			.y = 0,
			.width = window->Width(),
			.height = window->Height(),
		};
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		render(vp, flyCamera, restShader, lightSourceShader);
#endif

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
		flyCamera.ProcessMouseMovement((float) dx, (float) dy);
	}

	void Engine::Scroll_callback(double, double yoffset) {
		LOG(TRACE) << "mouse was scrolled " << yoffset << "\n";
		flyCamera.ProcessMouseScroll((float) yoffset);
	}

	Engine::~Engine() {
		LOG(TRACE) << "Engine is destroyed\n";
	}
}
