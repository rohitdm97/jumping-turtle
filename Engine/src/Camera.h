#pragma once

#include "log.h"
#include <iostream>

#include <glm/glm.hpp>

namespace render {
	class UniformStore;
}

namespace engine {
	namespace camera {
		enum Movement {
#define X(a) a,
#include "movements.enums"
#undef X
		};

		// todo does this even work?
		std::ostream& operator<<(std::ostream& os, Movement m);
	}

	class CameraMatrixProvider {
	public:
		virtual ~CameraMatrixProvider() {}
		virtual void SetUniforms(std::string name, render::UniformStore store, float) const = 0;
	};

	class Camera : public CameraMatrixProvider {
	private:
		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 worldUp;

		float yaw;
		float pitch;
		float speed;
		float mouseSensitivity;
		float zoom;

		void updateCameraVectors();
	public:
		Camera();
		~Camera() = default;
		Camera(const Camera&) = default;
		Camera& operator=(const Camera&) = default;
		Camera(Camera&&) = default;
		Camera& operator=(Camera&&) = default;

		Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
		void SetPosition(glm::vec3 position);

		// returns self
		Camera& WithPosition(glm::vec3);
		Camera& WithYaw(float);
		Camera& WithPitch(float);
		Camera& WithLookAt(glm::vec3);

		void SetUniforms(std::string name, render::UniformStore store, float) const;

		void ProcessKeyboard(camera::Movement dir, double delta);
		void ProcessMouseMovement(float xoffset, float yoffset, bool constraintPitch = true);
		void ProcessMouseScroll(float yoffset);

		// why does this even work? operator functions don't need namespace?
		friend util::logging::Logger& operator<<(util::logging::Logger& os, const engine::Camera& c);
	};

	class SideViewCamera : public CameraMatrixProvider {
	public:
		enum Side {
			TOP,
			BOTTOM,
			LEFT,
			RIGHT,
			FRONT,
			BACK,
		};
	private:
		float distance = 20.0f;
		Side side;
	public:
		SideViewCamera();
		~SideViewCamera() = default;
		SideViewCamera(const SideViewCamera&) = default;
		SideViewCamera& operator=(const SideViewCamera&) = default;
		SideViewCamera(SideViewCamera&&) = default;
		SideViewCamera& operator=(SideViewCamera&&) = default;

		SideViewCamera(Side);
		SideViewCamera WithDistance(float distance) const;

		void SetUniforms(std::string name, render::UniformStore store, float) const;

		friend util::logging::Logger& operator<<(util::logging::Logger& os, const engine::Camera& c);
	};

}
