#pragma once

#include <iostream>

#include <glm/glm.hpp>

namespace engine {
	namespace camera {
		enum Movement {
#define X(a) a,
#include "movements.enums"
#undef X
		};

		std::ostream& operator<<(std::ostream& os, Movement m);
	}
	class Camera {
	private:
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		float Yaw;
		float Pitch;
		float Speed;
		float MouseSensitivity;
		float Zoom;

		void updateCameraVectors();
	public:
		Camera();
		Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
		void SetPosition(glm::vec3 position);

		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectionMatrix(float);

		void ProcessKeyboard(camera::Movement dir, float delta);
		void ProcessMouseMovement(float xoffset, float yoffset, bool constraintPitch = true);
		void ProcessMouseScroll(float yoffset);
	};
}
