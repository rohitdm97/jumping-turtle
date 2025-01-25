#include "Shader.h"
#include "debug.h"
#include "log.h"
#include "types.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


#include "Camera.h"

void assertForNotFound(bool found, bool allowUnknowns) {
	if (!found) {
		assert(allowUnknowns && "unknown condition must have allowUnknowns");
	}
}

namespace render {
	ShaderLoader::ShaderLoader(std::string shaderName) : shaderName(shaderName) {
	}

	std::string loadContent(std::string shaderName, std::string ext) {
		std::string source;
		std::ifstream file;
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		file.open("shaders//" + shaderName + ext, std::ifstream::in);
		std::stringstream ss;
		ss << file.rdbuf();
		file.close();
		source = ss.str();
		return source;
	}

	unsigned int load(std::string sType, std::string source, GLuint type) {
		LOG(DEBUG) << "Compiling " << sType << " shader files\n";
		const char* sourceChars = source.c_str();
		unsigned int shader = glCreateShader(type);
		glShaderSource(shader, 1, &sourceChars, NULL);
		glCompileShader(shader);
		{
			int sucess;
			char infoLog[8192];
			glGetShaderiv(shader, GL_COMPILE_STATUS, &sucess);
			if (!sucess) {
				glGetShaderInfoLog(shader, 8192, NULL, infoLog);
				LOG(ERROR) << "Failed to compile " << sType << " shader " << infoLog;
				throw std::runtime_error("failed to compile " + sType + " shader");
			}
		}
		LOG(DEBUG) << "Success: " << sType << " shader compiled\n";
		return shader;
	}

	Shader ShaderLoader::Create() {
		LOG(DEBUG) << "[" << shaderName << "] Loading shader files\n";
		std::string vertexSource = loadContent(shaderName, ".vs");
		std::string fragmentSource = loadContent(shaderName, ".fs");
		unsigned int vertex = load("vertex", vertexSource, GL_VERTEX_SHADER);
		unsigned int fragment = load("fragment", fragmentSource, GL_FRAGMENT_SHADER);
		unsigned int ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		{
			int sucess;
			char infoLog[8192];
			glGetProgramiv(ID, GL_LINK_STATUS, &sucess);
			if (!sucess) {
				glGetProgramInfoLog(ID, 8192, NULL, infoLog);
				LOG(ERROR) << "Failed to link program " << infoLog;
				throw std::runtime_error("failed to link program");
			}
		}
		glValidateProgram(ID);
		{
			int sucess;
			char infoLog[8192];
			glGetProgramiv(ID, GL_VALIDATE_STATUS, &sucess);
			if (!sucess) {
				glGetProgramInfoLog(ID, 8192, NULL, infoLog);
				LOG(ERROR) << "Failed to validate program " << infoLog;
				throw std::runtime_error("failed to validate program");
			}
		}
		glDeleteShader(vertex);
		glDeleteShader(fragment);

		LOG(DEBUG) << "[" << shaderName << "] Shader loading completed\n";
		return Shader(ID);
	}

	Shader::Shader(): ID_(0), store(UniformStore(ID_)) {
	}

	Shader::Shader(unsigned int ID) : ID_(ID), store(UniformStore(ID)) {
	}

	void Shader::Activate() const {
		if (ID_) {
			glUseProgram(ID_);
		}
	}

	UniformStore Shader::Uniforms(bool allowUnknowns) const {
#ifdef ALLOW_UKNOWN_UNIFORMS
			return UniformStore(ID_, true);
#else
		if (allowUnknowns) {
			return UniformStore(ID_, true);
		}
		return store;
#endif // ALLOW_UKNOWN_UNIFORMS
	}

	void UniformStore::SetBool(const std::string& name, bool value, bool) {
		const auto [found, location] = getUniformLocation(name);
		assertForNotFound(found, allowUnknowns);
		if (found) glUniform1i(location, value ? 1 : 0);
	}

	void UniformStore::SetInt(const std::string& name, int value) {
		const auto [found, location] = getUniformLocation(name);
		assertForNotFound(found, allowUnknowns);
		if (found) glUniform1i(location, value);
	}

	void UniformStore::SetFloat(const std::string& name, float value) {
		const auto [found, location] = getUniformLocation(name);
		assertForNotFound(found, allowUnknowns);
		if (found) glUniform1f(location, value);
	}

	void UniformStore::SetVec3(const std::string& name, const glm::vec3& vec3) {
		const auto [found, location] = getUniformLocation(name);
		assertForNotFound(found, allowUnknowns);
		if (found) glUniform3fv(location, 1, glm::value_ptr(vec3));
	}

	void UniformStore::SetMat4(const std::string& name, glm::mat4 mat4) {
		const auto [found, location] = getUniformLocation(name);
		assertForNotFound(found, allowUnknowns);
		if (found) glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat4));
	}

	const unsigned int& Shader::ID() const {
		return this->ID_;
	}

	std::tuple<bool, unsigned int> UniformStore::getUniformLocation(const std::string& name) {
		auto it = uniformLocationsCache.find(name);
		if (it == uniformLocationsCache.end()) {
			int location = glGetUniformLocation(shaderID, name.c_str());
			if (location == -1) {
				if (allowUnknowns) {
					return std::tuple(false, 0);
				}
				throw std::invalid_argument("" + name + " is not found in the shader");
			}
			uniformLocationsCache[name] = location;
			return std::tuple(true, location);
		}
		return std::tuple(true, (*it).second);
	}

	UniformStore::UniformStore(const unsigned int shaderID) : shaderID(shaderID) {
	}

	UniformStore::UniformStore(const unsigned int shaderID, bool allowUnknowns) : shaderID(shaderID), allowUnknowns(allowUnknowns) {
	}

}
