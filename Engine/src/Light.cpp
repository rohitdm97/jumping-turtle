#include "Light.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

namespace static_data {
	namespace light {
		glm::vec3 g_positions[] = {
			glm::vec3(-0.5f, -0.5f, +0.5f),
			glm::vec3(+0.5f, -0.5f, +0.5f),
			glm::vec3(+0.5f, -0.5f, -0.5f),
			glm::vec3(-0.5f, -0.5f, -0.5f),
			glm::vec3(-0.5f, +0.5f, +0.5f),
			glm::vec3(+0.5f, +0.5f, +0.5f),
			glm::vec3(+0.5f, +0.5f, -0.5f),
			glm::vec3(-0.5f, +0.5f, -0.5f),
		};

		count_t g_vertices_size = 8;

		index_t g_indices[] = {
			4, 5, 6,
			4, 6, 7,
			0, 3, 2,
			0, 2, 1,

			0, 1, 5,
			0, 5, 4,
			3, 7, 6,
			3, 6, 2,

			1, 2, 6,
			1, 6, 5,
			0, 4, 7,
			0, 7, 3,
		};

		// 6 faces * 2 triangles * 3 vertices
		count_t g_indices_count = 6 * 2 * 3;
	}
}

using namespace static_data::light;

namespace render {

	void Light::calculateMatrix() {
		this->matrixCached = (glm::translate(glm::mat4(1.0), position)) * rotation * glm::scale(glm::mat4(1.0f), glm::vec3(scale));
	}

	void Light::initGlComps() noexcept {
		this->needsCleanUp = true;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * g_vertices_size, &g_positions[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_t) * g_indices_count, &g_indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	void Light::cleanUp() noexcept {
		if (needsCleanUp) {
			needsCleanUp = false;
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
			glDeleteVertexArrays(1, &VAO);
		}
	}

	Light::Light(glm::vec3 position, glm::vec3 color) {
		this->color = color;

		this->position = position;
		this->rotation = glm::mat4(1.0);
		this->scale = 0.1f;
		this->calculateMatrix();
		initGlComps();
	}

	Light::Light() {
		this->position = glm::vec3(0.0);
		this->rotation = glm::mat4(1.0);
		this->scale = 1.0f;
		this->calculateMatrix();
	}

	Light::~Light() noexcept {
		cleanUp();
	}

	Light::Light(Light&& o) noexcept {
		this->VAO = o.VAO;
		o.VAO = 0;
		this->VBO = o.VBO;
		o.VBO = 0;
		this->EBO = o.EBO;
		o.EBO = 0;

		this->needsCleanUp = true;
		o.needsCleanUp = false;

		this->position = o.position;
		this->rotation = o.rotation;
		this->scale = o.scale;
		this->matrixCached = o.matrixCached;

		this->color = o.color;
	}

	Light& Light::operator=(Light&& o) noexcept {
		if (this != &o) {
			this->VAO = o.VAO;
			o.VAO = 0;
			this->VBO = o.VBO;
			o.VBO = 0;
			this->EBO = o.EBO;
			o.EBO = 0;

			this->needsCleanUp = true;
			o.needsCleanUp = false;

			this->position = o.position;
			this->rotation = o.rotation;
			this->scale = o.scale;
			this->matrixCached = o.matrixCached;

			this->color = o.color;
		}
		return *this;
	}

	const glm::vec3& Light::Color() const {
		return this->color;
	}

	const glm::vec3& Light::Position() const {
		return this->position;
	}

	void Light::SetPosition(const glm::vec3& position) {
		this->position = position;
		this->calculateMatrix();
	}

	void Light::SetColor(const glm::vec3& color) {
		this->color = color;
	}

	void Light::Rotate(const glm::vec3& axis, float angleInRadians) {
		this->rotation = glm::rotate(this->rotation, angleInRadians, axis);
		this->calculateMatrix();
	}

	void Light::Attach(render::Shader& shader) const {
		shader.Activate();
		glBindVertexArray(VAO);
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			int p = glGetAttribLocation(shader.ID(), "Position");
			assert(p != -1 && "expects Position attrib used by the shader");
			glVertexAttribPointer(p, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(p);
		}
		glBindVertexArray(0);
	}

	void Light::Render(render::Shader& shader) const {
		glBindVertexArray(VAO);
		shader.Uniforms(true).SetMat4("Model", matrixCached);
		shader.Uniforms(true).SetMat4("Mesh", glm::mat4(1.0));
		shader.Uniforms(true).SetLight("light", *this);
		glDrawElements(GL_TRIANGLES, g_indices_count, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

}
