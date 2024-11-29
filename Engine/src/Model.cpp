#include "Model.h"

#include "glm/gtc/matrix_transform.hpp"

#include "Shader.h"

namespace comp {
	void Model::calculateMatrix() {
		this->matrixCached = position * rotation * (scale * glm::mat4(1.0f));
	}
	Model::Model(Mesh& mesh) {
		this->mesh = std::move(mesh);
		this->position = glm::mat4(1.0f);
		this->rotation = glm::mat4(1.0f);
		this->scale = 1.0f;
		this->calculateMatrix();
	}
	void Model::SetPosition(glm::vec3 position) {
		this->position = glm::translate(glm::mat4(), position);
		this->calculateMatrix();
	}
	void Model::Rotate(glm::vec3 axis, float angleInRadians) {
		this->rotation = glm::rotate(this->rotation, angleInRadians, axis);
		this->calculateMatrix();
	}
	glm::mat4 Model::ModelMatrix() const {
		return matrixCached;
	}
	void Model::Attach(render::Shader& shader) {
		mesh.Attach(shader);
	}
	void Model::Render(render::Shader& shader) {
		shader.SetMat4("Model", matrixCached);
		mesh.Render();
	}
}
