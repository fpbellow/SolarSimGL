#include "../Headers/resource_manager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../Headers/stb_image.h"

#include <iostream>
#include <sstream>
#include <fstream>


//instatiate static variables
std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;


Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}


Shader ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}


Texture2D ResourceManager::LoadTexture(const char* file, std::string name)
{
	Textures[name] = loadTextureFromFile(file);
	return Textures[name];
}


Texture2D ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}


void ResourceManager::Clear()
{
	//delete all shaders
	for (auto iterator : Shaders)
		glDeleteProgram(iterator.second.id);
	
	for (auto iterator : Textures)
		glDeleteTextures(1, &iterator.second.id);
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        
        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();


        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();


    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}


Texture2D ResourceManager::loadTextureFromFile(const char* file)
{
    Texture2D texture;
    /*if (alpha) 
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }*/

    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    if (data)
    {
        
        if (nrChannels == 1)
        {
            texture.Internal_Format = GL_RED;
            texture.Image_Format = GL_RED;
        }
        else if (nrChannels == 3)
        {
            texture.Internal_Format = GL_RGB;
            texture.Image_Format = GL_RGB;
        }
        else if (nrChannels == 4)
        {
            texture.Internal_Format = GL_RGBA;
            texture.Image_Format = GL_RGBA;
        }
    }

    texture.Generate(width, height, data);
    stbi_image_free(data);

    return texture;
}