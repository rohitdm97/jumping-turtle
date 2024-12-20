#pragma once

#include "log.h"
#include <iostream>

#include <glm/glm.hpp>

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
	class Camera {
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
		Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
		void SetPosition(glm::vec3 position);

		const glm::vec3& Position() const;
		glm::mat4 GetViewMatrix() const;
		glm::mat4 GetProjectionMatrix(float) const;

		void ProcessKeyboard(camera::Movement dir, double delta);
		void ProcessMouseMovement(float xoffset, float yoffset, bool constraintPitch = true);
		void ProcessMouseScroll(float yoffset);

		// why does this even work? operator functions don't need namespace?
		friend util::logging::Logger& operator<<(util::logging::Logger& os, const engine::Camera& c);
	};
}
