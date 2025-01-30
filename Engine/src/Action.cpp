#include "Action.h"

#include <glad/glad.h>
#include <glm/vec3.hpp>

#include "Window.h"
#include "Camera.h"
#include "entities.h"
#include "Light.h"

#include "log.h"

namespace game {
	namespace actions {
		ExitGame::ExitGame(render::Window& w) : window(w) {
		}
		void ExitGame::Execute() {
			glfwSetWindowShouldClose(window.Ref(), true);
		}
		CameraMove::CameraMove(engine::camera::Movement d, engine::Camera& c) : direction(d), camera(c) {
		}
		void CameraMove::Execute() {
			camera.ProcessKeyboard(direction);
		}
		CameraCaptureStats::CameraCaptureStats(engine::Camera& c) : camera(c) {
		}
		void CameraCaptureStats::Execute() {
			LOG(DEBUG) << "camera: " << camera << "\n";
		}
		SpawnParticle::SpawnParticle(
			engine::Camera& c,
			entities::System& e,
			const comp::Model& s
		) : camera(c), entities(e), spawner(s) {
		}
		void SpawnParticle::Execute() {
			auto ID = entities.CreateEntity();
			entities.ai[ID].dummy = 4;
			entities.physics[ID].position = camera.Position();
			glm::vec3 direction = camera.Front() + glm::vec3(0, -1, 0);
			entities.physics[ID].velocity = glm::normalize(direction);
			entities.renders[ID].VAO = spawner.GetVAO();
		}
		
		ChangeLightType::ChangeLightType(render::Light& l) : light(l) {
		}

		void ChangeLightType::Execute() {
			light.ChangeLightType();
		}

	}
}
