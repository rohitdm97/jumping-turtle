#pragma once

#include "Mesh.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

namespace render {
	class Shader;
}

namespace comp {
	class Model {
	private:
		float scale;
		glm::mat4 rotation;
		glm::mat4 position;
		glm::mat4 matrixCached;
		Mesh mesh;

		void calculateMatrix();
	public:
		Model(Mesh& mesh);
		void SetPosition(glm::vec3 position);
		void Rotate(glm::vec3 axis, float angleInRadians);

		glm::mat4 ModelMatrix() const;

		void Attach(render::Shader& shader);
		void Render(render::Shader& shader);
	};
}
