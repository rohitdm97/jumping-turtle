#include "Camera.h"
#include "log.h"

#include <glm/gtc/matrix_transform.hpp>

#include <string>

#include "Shader.h"

constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 10.0f / 60.0f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM = 45.0f;

constexpr float EPSILON = 1e-4f;

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
		glm::vec3 front_{};
		front_.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front_.y = sin(glm::radians(pitch));
		front_.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		this->front = glm::normalize(front_);
		this->right = glm::normalize(glm::cross(front, worldUp));
		this->up = glm::normalize(glm::cross(right, front));
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

	void Camera::SetPosition(glm::vec3 position_) {
		this->position = position_;
	}

	const glm::vec3& Camera::Position() const {
		return position;
	}
	const glm::vec3& Camera::Front() const {
		return front;
	}
	const glm::vec3& Camera::Up() const {
		return up;
	}
	const glm::vec3& Camera::Right() const {
		return right;
	}
	const glm::vec3& Camera::WorldUp() const {
		return worldUp;
	}

	Camera& Camera::WithPosition(glm::vec3 position_) {
		this->position = position_;
		return *this;
	}

	Camera& Camera::WithYaw(float yaw_) {
		this->yaw = yaw_;
		return *this;
	}

	Camera& Camera::WithPitch(float pitch_) {
		this->pitch = pitch_;
		return *this;
	}

	float cap(float min, float val, float max) {
		return val < min ? min : (val > max ? max : val);
	}

	Camera& Camera::WithLookAt(glm::vec3 target) {
		auto a = target - position;
		auto b = glm::normalize(a);
		const glm::vec3 front_ = glm::normalize(target - position);
		// [-pi/2 pi/2]
		pitch = cap(-89.9f, glm::degrees(glm::asin(front_.y)), 89.9f);
		{
			const float front_y_expected = glm::sin(glm::radians(pitch));
			assert((glm::abs(front_.y - front_y_expected) <= EPSILON) && "lookAt calculations for front.y are not matching");
		}
		const float yawR = glm::atan(front_.z, front_.x);
		const float yawD = glm::degrees(yawR);
		yaw = yawD;
		{
			const float front_z_expected = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			assert((glm::abs(front_.z - front_z_expected) <= EPSILON) && "lookAt calculations for front.z are not matching");
		}
		{
			const float front_x_expected = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			assert((glm::abs(front_.x - front_x_expected) <= EPSILON) && "lookAt calculations for front.x are not matching");
		}

		updateCameraVectors();
		return *this;
	}

	void Camera::SetUniforms(std::string name, render::UniformStore store, float aspectRatio) const {
		const glm::mat4 view = glm::lookAt(position, position + front, up);
		const glm::mat4 projection = glm::perspective(glm::radians(zoom), aspectRatio, 0.1f, 100.0f);

		store.SetVec3(name + ".position", position);
		store.SetMat4(name + ".view", view);
		store.SetMat4(name + ".projection", projection);
	}

	void Camera::ProcessKeyboard(camera::Movement dir) {
		LOG(TRACE) << "Processing Keyboard event " << dir << "\n"; 
		float dx = speed;
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
		if (yaw > 180.0f) {
			yaw = yaw - 360.0f;
		}
		if (yaw < -180.0f) {
			yaw = yaw + 360.0f;
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
		os << "camera: Position = " << c.position << ", WorldUp = " << c.worldUp << ", Yaw = " << c.yaw << ", Pitch = " << c.pitch << ", Front = " << c.front;
		return os;
	}

	SideViewCamera::SideViewCamera() : SideViewCamera(TOP) {
	}

	SideViewCamera::SideViewCamera(Side s) : side(s) {
	}

	SideViewCamera SideViewCamera::WithDistance(float d) const {
		SideViewCamera s(this->side);
		s.distance = d;
		return s;
	}

	void SideViewCamera::SetUniforms(std::string name, render::UniformStore store, float) const {
		glm::vec3 position(0.0f);
		glm::vec3 up(0.0f);
		glm::mat4 view;
		switch (side) {
		case engine::SideViewCamera::TOP:
			position = glm::vec3(0, distance, 0);
			up = glm::vec3(0, 0, -1);
			view = glm::mat4(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
			break;
		case engine::SideViewCamera::BOTTOM:
			position = glm::vec3(0, -distance, 0);
			up = glm::vec3(0, 0, 1);
			view = glm::mat4(
			   -1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f,-1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
			break;
		case engine::SideViewCamera::LEFT:
			position = glm::vec3(distance, 0, 0);
			up = glm::vec3(0, 1, 0);
			view = glm::mat4(
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f,-1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
			break;
		case engine::SideViewCamera::RIGHT:
			position = glm::vec3(-distance, 0, 0);
			up = glm::vec3(0, 1, 0);
			view = glm::mat4(
				0.0f, 0.0f, -1.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
			break;
		case engine::SideViewCamera::FRONT:
			position = glm::vec3(0, 0, distance);
			up = glm::vec3(0, 1, 0);
			view = glm::mat4(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
			break;
		case engine::SideViewCamera::BACK:
			position = glm::vec3(0, 0, -distance);
			up = glm::vec3(0, 1, 0);
			view = glm::mat4(
			   -1.0f, 0.0f, 0.0f, 0.0f,
				0.0f,-1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
			break;
		default:
			assert(false);
			return;
		}

		store.SetVec3(name + ".position", position);
		store.SetMat4(name + ".view", view);
		float range = 5;
		store.SetMat4(name + ".projection", glm::ortho<float>(-range, range, -range, range, -range, range));
	}

}
