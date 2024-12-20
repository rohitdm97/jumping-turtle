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
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		this->front = glm::normalize(front);
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}

	Camera::Camera() : Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), YAW, PITCH) {
	}

	Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
		this->position = position;
		this->worldUp = up;
		this->yaw = yaw;
		this->pitch = pitch;

		this->speed = SPEED;
		this->mouseSensitivity = SENSITIVITY;
		this->zoom = ZOOM;
		updateCameraVectors();
	}

	void Camera::SetPosition(glm::vec3 position) {
		this->position = position;
	}

	const glm::vec3& Camera::Position() const {
		return this->position;
	}

	glm::mat4 Camera::GetViewMatrix() const {
		return glm::lookAt(position, position + front, up);
	}

	glm::mat4 Camera::GetProjectionMatrix(float aspectRatio) const {
		return glm::perspective(glm::radians(zoom), aspectRatio, 0.1f, 100.0f);
	}

	void Camera::ProcessKeyboard(camera::Movement dir, double delta) {
		LOG(TRACE) << "Processing Keyboard event " << dir << "\n"; 
		float dx = speed * (float) delta;
		switch (dir) {
		case engine::camera::FORWARD:
			position += front * dx;
			break;
		case engine::camera::BACKWARD:
			position -= front * dx;
			break;
		case engine::camera::LEFT:
			position -= right * dx;
			break;
		case engine::camera::RIGHT:
			position += right * dx;
			break;
		default:
			throw std::invalid_argument("Unknown direction");
		}
	}

	void Camera::ProcessMouseMovement(float xoffset_, float yoffset_, bool constraintPitch) {
		float xoffset = xoffset_ * mouseSensitivity;
		float yoffset = yoffset_ * mouseSensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (constraintPitch) {
			if (pitch > 89.0f) {
				pitch = 89.0f;
			}
			if (pitch < -89.0f) {
				pitch = -89.0f;
			}
		}

		LOG(TRACE) << "pitch = " << pitch << " yaw = " << yaw << "\n";
		updateCameraVectors();
	}

	void Camera::ProcessMouseScroll(float yoffset) {
		zoom -= (float) yoffset;
		if (zoom < 1.0f) {
			zoom = 1.0f;
		}
		if (zoom > 45.0f) {
			zoom = 45.0f;
		}

		LOG(TRACE) << "Zoom = " << zoom << "\n";
	}

	util::logging::Logger& operator<<(util::logging::Logger& os, const engine::Camera& c) {
		os << "camera: Position = " << c.position << ", WorldUp = " << c.worldUp << ", Yaw = " << c.yaw << ", Pitch = " << c.pitch;
		return os;
	}

}
