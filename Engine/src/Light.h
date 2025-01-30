#pragma once
#include "types.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <string>

namespace render {
	class Shader;
	class UniformStore;

	class Light {
	public:
		enum Type {
			UNKNOWN = 0,
			DIRECTION_LIGHT = 1,
			POINT_LIGHT = 2,
			SPOT_LIGHT = 4,
		};
	private:
		glObject VAO, VBO, EBO;
		float scale;
		int delay = 0;
		Type type = POINT_LIGHT;
		glm::mat4 rotation;
		glm::vec3 position;
		glm::mat4 matrixCached;

		void calculateMatrix();

		glm::vec3 color;

		bool needsCleanUp;
		void initGlComps() noexcept;
		void cleanUp() noexcept;
	public:
		explicit Light(glm::vec3 position, glm::vec3 color);
		Light();
		~Light() noexcept;

		Light(const Light& o) = delete;
		Light& operator=(const Light& o) = delete;
		Light(Light&& o) noexcept;
		Light& operator=(Light&& o) noexcept;

		const glm::vec3& Color() const;
		const glm::vec3& Position() const;

		void SetUniforms(std::string name, render::UniformStore store) const;

		void SetPosition(const glm::vec3& position);
		void SetColor(const glm::vec3& color);
		void Rotate(const glm::vec3& axis, float angleInRadians);

		void Render(render::Shader& shader) const;
		void ChangeLightType();
	};
}
