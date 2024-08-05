#ifndef RESOURCE_MANAGER_H 
#define RESOURCE_MANAGER_H

#include <glad/glad.h>
#include "../Headers/texture.h"
#include "../Headers/shader.h"

#include <map>
#include <string>

//static singleton class that hosts functions for loading textures and shaders
//assets stored for future reference by string handles
//functions and resources are static/ no public constructor

class ResourceManager
{
public:

	//resource storage
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture2D> Textures;

	//load shader prgm from file, loading individual shaders source codes
	static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);

	//retrives stored shader
	static Shader GetShader(std::string name);

	//load texture from file
	static Texture2D LoadTexture(const char* file, std::string name);

	//retrieves a stored texture
	static Texture2D GetTexture(std::string name);

	//de-allocate loaded resources
	static void Clear();


private:
	//private constructor
	ResourceManager() {}

	//load & generate shader from file
	static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);

	//load single texture from file
	static Texture2D loadTextureFromFile(const char* file);
};

#endif