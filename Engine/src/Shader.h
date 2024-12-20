#pragma once

#include <string>
#include <map>
#include <tuple>

#include "glm/matrix.hpp"

namespace engine {
	class Camera;
}

namespace render {

	class Shader;
	class Light;

	enum ShaderType {
		NONE = 0,
		VERTEX = 1,
		FRAGMENT = 2,
	};

	class ShaderLoader {
	private:
		std::string shaderName;
	public:
		ShaderLoader(std::string filename);
		Shader Create();
	};

	class UniformStore {
	private:
		unsigned int shaderID;
		bool allowUnknowns = false;

		std::map<std::string, unsigned int> uniformLocationsCache;

		std::tuple<bool, unsigned int> getUniformLocation(const std::string& name);
	public:
		UniformStore(const unsigned int);
		UniformStore(const unsigned int, bool);

		void SetInt(const std::string& name, int value);
		void SetVec3(const std::string& name, const glm::vec3&);
		void SetMat4(const std::string& name, glm::mat4);
		void SetCamera(const std::string& name, const engine::Camera& camera, float);
		void SetLight(const std::string& name, const render::Light&);
	};

	class Shader {
	private:
		unsigned int ID_;
		UniformStore store;
	public:
		// no-op
		Shader();
		Shader(unsigned int ID);

		void Activate() const;

		UniformStore Uniforms(bool allowUnknowns = false);

		const unsigned int& ID() const;
	};
}
