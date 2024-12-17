#include "Camera.h"
#include "log.h"

#include <glm/gtc/matrix_transform.hpp>

#include <string>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

util::logging::Logger& operator<<(util::logging::Logger& os, engine::camera::Movement m) {
	switch (m) {
#define X(a) case engine::camera::a: os << #a; return os;
#include "movements.enums"
#undef X
	default:
		throw new std::invalid_argument("Unknown enum Movement " + std::to_string(m));
	}
}

std::ostream& operator<<(std::ostream& os, engine::camera::Movement m) {
	switch (m) {
#define X(a) case engine::camera::a: os << #a; return os;
#include "movements.enums"
#undef X
	default:
		throw new std::invalid_argument("Unknown enum Movement " + std::to_string(m));
	}
}

namespace engine {
	namespace camera {
	}

	void Camera::updateCameraVectors() {
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

	Camera::Camera() : Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), YAW, PITCH) {
	}

	Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;

		this->Speed = SPEED;
		this->MouseSensitivity = SENSITIVITY;
		this->Zoom = ZOOM;
		updateCameraVectors();
	}

	void Camera::SetPosition(glm::vec3 position) {
		this->Position = position;
	}

	glm::mat4 Camera::GetViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
	}

	glm::mat4 Camera::GetProjectionMatrix(float aspectRatio) {
		return glm::perspective(glm::radians(Zoom), aspectRatio, 0.1f, 100.0f);
	}

	void Camera::ProcessKeyboard(camera::Movement dir, double delta) {
		LOG(TRACE) << "Processing Keyboard event " << dir << "\n"; 
		float dx = Speed * (float) delta;
		switch (dir) {
		case engine::camera::FORWARD:
			Position += Front * dx;
			break;
		case engine::camera::BACKWARD:
			Position -= Front * dx;
			break;
		case engine::camera::LEFT:
			Position -= Right * dx;
			break;
		case engine::camera::RIGHT:
			Position += Right * dx;
			break;
		default:
			throw std::invalid_argument("Unknown direction");
		}
	}

	void Camera::ProcessMouseMovement(float xoffset_, float yoffset_, bool constraintPitch) {
		float xoffset = xoffset_ * MouseSensitivity;
		float yoffset = yoffset_ * MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		if (constraintPitch) {
			if (Pitch > 89.0f) {
				Pitch = 89.0f;
			}
			if (Pitch < -89.0f) {
				Pitch = -89.0f;
			}
		}

		LOG(TRACE) << "pitch = " << Pitch << " yaw = " << Yaw << "\n";
		updateCameraVectors();
	}

	void Camera::ProcessMouseScroll(float yoffset) {
		Zoom -= (float) yoffset;
		if (Zoom < 1.0f) {
			Zoom = 1.0f;
		}
		if (Zoom > 45.0f) {
			Zoom = 45.0f;
		}

		LOG(TRACE) << "Zoom = " << Zoom << "\n";
	}

}
