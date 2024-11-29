#include "Shader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "log.h"

namespace render {
	ShaderLoader::ShaderLoader(std::string shaderName) : shaderName(shaderName) {
	}

	std::string loadContent(std::string shaderName, std::string ext) {
		std::string source;
		std::ifstream file;
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		file.open("shaders//" + shaderName + ext);
		std::stringstream ss;
		ss << file.rdbuf();
		file.close();
		source = ss.str();
		return source;
	}

	unsigned int load(std::string sType, std::string source, GLuint type) {
		const char* sourceChars = source.c_str();
		unsigned int shader = glCreateShader(type);
		glShaderSource(shader, 1, &sourceChars, NULL);
		glCompileShader(shader);
		{
			int sucess;
			char infoLog[1024];
			glGetShaderiv(shader, GL_COMPILE_STATUS, &sucess);
			if (!sucess) {
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				LOG(ERROR) << "Failed to compile " << sType << " shader " << infoLog;
				throw std::runtime_error("failed to compile " + sType + " shader");
			}
		}
		return shader;
	}

	Shader ShaderLoader::Create() {
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
			char infoLog[1024];
			glGetProgramiv(ID, GL_LINK_STATUS, &sucess);
			if (!sucess) {
				glGetProgramInfoLog(ID, 1024, NULL, infoLog);
				LOG(ERROR) << "Failed to link program " << infoLog;
				throw std::runtime_error("failed to link program");
			}
		}
		glValidateProgram(ID);
		{
			int sucess;
			char infoLog[1024];
			glGetProgramiv(ID, GL_VALIDATE_STATUS, &sucess);
			if (!sucess) {
				glGetProgramInfoLog(ID, 1024, NULL, infoLog);
				LOG(ERROR) << "Failed to validate program " << infoLog;
				throw std::runtime_error("failed to validate program");
			}
		}
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		return Shader(ID);
	}

	Shader::Shader(): ID_(0) {
	}

	Shader::Shader(unsigned int ID) : ID_(ID) {
	}

	void Shader::Activate() {
		if (ID_) {
			glUseProgram(ID_);
		}
	}

	unsigned int Shader::getUniformLocation(const std::string& name) {
		auto it = uniformLocationsCache.find(name);
		if (it == uniformLocationsCache.end()) {
			int location = glGetUniformLocation(ID_, name.c_str());
			if (location == -1) {
				throw std::invalid_argument("" + name + " is not found in the shader");
			}
			uniformLocationsCache[name] = location;
			return location;
		}
		return (*it).second;
	}

	void Shader::SetBool(const std::string& name, bool value) {
		glUniform1i(getUniformLocation(name), (int) value);
	}

	void Shader::SetInt(const std::string& name, int value) {
		glUniform1i(getUniformLocation(name), value);
	}

	void Shader::SetFloat(const std::string& name, float value) {
		glUniform1f(getUniformLocation(name), value);
	}

	void Shader::SetVec4(const std::string& name, float values[]) {
		glUniform4f(getUniformLocation(name), values[0], values[1], values[2], values[3]);
	}

	void Shader::SetMat4(const std::string& name, glm::mat4 mat4) {
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat4));
	}

	const unsigned int& Shader::ID() const {
		return this->ID_;
	}

}
