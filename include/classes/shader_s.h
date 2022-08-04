#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <spdlog/spdlog.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

class Shader
{
private:
	bool shaderInitialized = false;
	std::map<GLenum, std::string> shaderFileMap{};

public:
	unsigned int ID;
	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	Shader() {

	}

	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
	{
		ID = create_shader_program_from_files(vertexPath, fragmentPath, geometryPath);
		shaderInitialized = true;
	}

	std::string read_shader_file_to_str(const char* shader_filename) {
		std::string shaderCode;
		std::ifstream shaderFile;

		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			// open files
			shaderFile.open(shader_filename);
			std::stringstream shaderStream;

			// read file's buffer contents into streams
			shaderStream << shaderFile.rdbuf();

			// close file handlers
			shaderFile.close();

			// convert stream into string
			shaderCode = shaderStream.str();
		} catch (std::ifstream::failure& e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            spdlog::get("logger")->error("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
		}

		return shaderCode;
	}

	unsigned int create_shader_from_file(const char* shader_filename, GLenum shader_type) {
		unsigned int shader;

		auto shaderFileString = read_shader_file_to_str(shader_filename);
		auto shaderCodeStr = shaderFileString.c_str();

        spdlog::get("logger")->info("Creating shader from file: {0}, with type {1}", shader_filename, shader_type);

		// vertex shader
		shader = glCreateShader(shader_type);
		glShaderSource(shader, 1, &shaderCodeStr, NULL);
		glCompileShader(shader);

		// Check for errors
		switch (shader_type) {
		case GL_VERTEX_SHADER:
			checkCompileErrors(shader, "VERTEX");
			break;
		case GL_FRAGMENT_SHADER:
			checkCompileErrors(shader, "FRAGMENT");
			break;
		case GL_GEOMETRY_SHADER:
			checkCompileErrors(shader, "GEOMETRY");
			break;
		default:
			checkCompileErrors(shader, "UNKNOWN");
			break;
		}

        spdlog::get("logger")->info("Shader creation successful");

		return shader;
	}

	GLuint create_shader_program_from_files(const char* vertex_shader_filename, const char* fragment_shader_filename, const char* geometryPath = nullptr) {
		unsigned int vertex, fragment, geometry;
        
        spdlog::get("logger")->info("Initializing shader with vertex shader: {0}, fragment shader: {1}, geometry shader: {2}", vertex_shader_filename, fragment_shader_filename, geometryPath ? geometryPath : "N/A");

		vertex = create_shader_from_file(vertex_shader_filename, GL_VERTEX_SHADER);
		shaderFileMap[GL_VERTEX_SHADER] = vertex_shader_filename;

		fragment = create_shader_from_file(fragment_shader_filename, GL_FRAGMENT_SHADER);
		shaderFileMap[GL_FRAGMENT_SHADER] = fragment_shader_filename;

		if (geometryPath != nullptr) {
			geometry = create_shader_from_file(geometryPath, GL_GEOMETRY_SHADER);
			shaderFileMap[GL_GEOMETRY_SHADER] = geometryPath;
		}

		// shader Program
		unsigned int program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);

		if (geometryPath != nullptr) {
			glAttachShader(program, geometry);
		}

		glLinkProgram(program);
		checkCompileErrors(program, "PROGRAM");

		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);

		if (geometryPath != nullptr) {
			glDeleteShader(geometry);
		}

		return program;
	}

	GLuint reload_shader_program_from_files(const char* vertex_shader_filename, const char* fragment_shader_filename, const char* geometry_shader_filename = nullptr)  {
		assert(vertex_shader_filename && fragment_shader_filename);

        spdlog::get("logger")->info("Reloading shaders");

		GLuint reloaded_program = create_shader_program_from_files(vertex_shader_filename, fragment_shader_filename, geometry_shader_filename);

		if (reloaded_program) {
			glDeleteProgram(ID);
			ID = reloaded_program;
			return ID;
		}

		return 0;
	}

	GLuint hot_reload_shaders() {
		auto vertPath = this->shaderFileMap.at(GL_VERTEX_SHADER).c_str();
		auto fragPath = this->shaderFileMap.at(GL_FRAGMENT_SHADER).c_str();
		auto geomPath = this->shaderFileMap.find(GL_GEOMETRY_SHADER) != this->shaderFileMap.end() ? this->shaderFileMap.at(GL_VERTEX_SHADER).c_str() : nullptr;

        spdlog::get("logger")->info("Hot reloading shaders");

		return reload_shader_program_from_files(
			vertPath,
			fragPath,
			geomPath
		);
	}

	// Setters
	void setID(GLuint _ID) {
		this->ID = _ID;
	}

	// Getters
	std::map<GLuint, std::string> getShaderFileMap() {
		return this->shaderFileMap;
	}

	// activate the shader
	// ------------------------------------------------------------------------
	void use()
	{
		glUseProgram(ID);
	}

	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	// ------------------------------------------------------------------------
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	// ------------------------------------------------------------------------
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	// ------------------------------------------------------------------------
	void setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}

	void setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}

	// ------------------------------------------------------------------------
	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}

	void setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}

	// ------------------------------------------------------------------------
	void setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}

	void setVec4(const std::string& name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}

	// ------------------------------------------------------------------------
	void setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	// ------------------------------------------------------------------------
	void setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	// ------------------------------------------------------------------------
	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

private:
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(GLuint shader, std::string type)
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                spdlog::get("logger")->error("ERROR::SHADER_COMPILATION_ERROR of type: {0}\n{1}", type, infoLog);
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                spdlog::get("logger")->error("ERROR::PROGRAM_LINKING_ERROR of type: {0}\n{1}", type, infoLog);
			}
		}
	}
};
#endif