#include "../Headers/shader.h"

#include <iostream>

Shader& Shader::Use()
{
	glUseProgram(this->id);
	return *this;
}

void Shader::Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
	unsigned int sVertex, sFragment, sGeometry;

	//vertexx shader
	sVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(sVertex, 1, &vertexSource, NULL);
	glCompileShader(sVertex);
	verifyShader(sVertex, "VERTEX");

	//frag shader
	sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &fragmentSource, NULL);
	glCompileShader(sFragment);
	verifyShader(sFragment, "FRAGMENT");

	//geometry shader (if provided)
	if(geometrySource != nullptr)
	{
		sGeometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(sGeometry, 1, &geometrySource, NULL);
		glCompileShader(sGeometry);
		verifyShader(sGeometry, "GEOMETRY");
	}

	//shader program
	this->id = glCreateProgram();
	glAttachShader(this->id, sVertex);
	glAttachShader(this->id, sFragment);
	if (geometrySource != nullptr)
		glAttachShader(this->id, sGeometry);
	glLinkProgram(this->id);
	verifyShader(this->id, "PROGRAM");
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
	if (geometrySource != nullptr)
		glDeleteShader(sGeometry);
}

void Shader::SetBool(const char* name, bool value, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform1f(glGetUniformLocation(this->id, name), value);
}

void Shader::SetInt(const char* name, int value, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform1i(glGetUniformLocation(this->id, name), value);
}

void Shader::SetFloat(const char* name, float value, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform1f(glGetUniformLocation(this->id, name), value);
}



void Shader::SetVec2f(const char* name, float x, float y, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform2f(glGetUniformLocation(this->id, name), x, y);
}
void Shader::SetVec2f(const char* name, const glm::vec2& value, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform2f(glGetUniformLocation(this->id, name), value.x, value.y);
}


void Shader::SetVec3f(const char* name, float x, float y, float z, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->id, name), x, y, z);
}

void Shader::SetVec3f(const char* name, const glm::vec3& value, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->id, name), value.x, value.y, value.z);
}


void Shader::SetVec4f(const char* name, float x, float y, float z, float w, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform4f(glGetUniformLocation(this->id, name), x, y, z, w);
}

void Shader::SetVec4f(const char* name, const glm::vec4& value, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform4f(glGetUniformLocation(this->id, name), value.x, value.y, value.z, value.w);
}


void Shader::SetMat2(const char* name, const glm::mat2& matrix, bool useShader)
{
	if (useShader)
		this->Use();
	glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, false, glm::value_ptr(matrix));
}

void Shader::SetMat3(const char* name, const glm::mat3& matrix, bool useShader)
{
	if (useShader)
		this->Use();
	glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, false, glm::value_ptr(matrix));
}

void Shader::SetMat4(const char* name, const glm::mat4& matrix, bool useShader)
{
	if (useShader)
		this->Use();
	glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, false, glm::value_ptr(matrix));
}



void Shader::verifyShader(unsigned int object, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}
}