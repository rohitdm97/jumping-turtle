#pragma once

#include <string>
#include <map>

#include "glm/matrix.hpp"

namespace render {

	class Shader;

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

	class Shader {
	private:
		std::map<std::string, unsigned int> uniformLocationsCache;
		unsigned int ID_;

		unsigned int getUniformLocation(const std::string& name);
	public:
		// no-op
		Shader();
		Shader(unsigned int ID);
		void Activate();
		void SetBool(const std::string& name, bool value);
		void SetInt(const std::string& name, int value);
		void SetFloat(const std::string& name, float value);
		// todo deprecate
		void SetVec4(const std::string& name, float values[]);
		void SetMat4(const std::string& name, glm::mat4);

		const unsigned int& ID() const;
	};
}
