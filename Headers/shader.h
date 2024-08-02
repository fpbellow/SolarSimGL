#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	//state
	unsigned int id;

	//constructor
	Shader() {}
	
	//set current shader as active
	Shader& Use();

	//compiles shader from source code
	void Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);

	//utility functions
	void SetBool(const char* name, bool value, bool useShader = false);
	void SetInt(const char* name, int value, bool useShader = false);
	void SetFloat(const char* name, float value, bool useShader = false);

	void SetVec2f(const char* name, float x, float y, bool useShader = false);
	void SetVec2f(const char* name, const glm::vec2& value, bool useShader = false);

	void SetVec3f(const char* name, float x, float y, float z, bool useShader = false);
	void SetVec3f(const char* name, const glm::vec3& value, bool useShader = false);

	void SetVec4f(const char* name, float x, float y, float z, float w, bool useShader = false);
	void SetVec4f(const char* name, const glm::vec4& value, bool useShader = false);

	void SetMat2(const char* name, const glm::mat2& matrix, bool useShader = false);
	void SetMat3(const char* name, const glm::mat3& matrix, bool useShader = false);
	void SetMat4(const char* name, const glm::mat4& matrix, bool useShader = false);

private:
	void verifyShader(unsigned int object, std::string type);
};


#endif