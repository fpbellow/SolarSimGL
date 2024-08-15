#ifndef RESOURCE_MANAGER_H 
#define RESOURCE_MANAGER_H

#include <glad/glad.h>
#include "texture.h"
#include "shader.h"
#include "cubemap.h"

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
	static std::map<std::string, Cubemap> Cubemaps;

	//load shader prgm from file, loading individual shaders source codes
	static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);

	//retrives stored shader
	static Shader GetShader(std::string name);

	//load texture from file
	static Texture2D LoadTexture(const char* file, std::string name);

	//retrieves a stored texture
	static Texture2D GetTexture(std::string name);

	//load cubemap from vector of textures
	static Cubemap LoadCubemap(std::vector<std::string> faces, std::string name);

	//retrieves stored cubemap
	static Cubemap GetCubemap(std::string name);

	//de-allocate loaded resources
	static void Clear();


private:
	//private constructor
	ResourceManager() {}

	//load & generate shader from file
	static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);

	//load single texture from file
	static Texture2D loadTextureFromFile(const char* file);

	//load cubemap from vector of filepaths
	static Cubemap loadCubemapFromFileVector(std::vector<std::string> faces);
};

#endif