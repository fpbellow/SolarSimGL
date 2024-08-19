#include "../Headers/resource_manager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../Headers/stb_image.h"

#include <iostream>
#include <sstream>
#include <fstream>


//instatiate static variables
std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Cubemap> ResourceManager::Cubemaps;


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


Cubemap ResourceManager::LoadCubemap(std::vector<std::string> faces, std::string name)
{
    Cubemaps[name] = loadCubemapFromFileVector(faces);
    return Cubemaps[name];
}


Cubemap ResourceManager::GetCubemap(std::string name)
{
    return Cubemaps[name];
}


void ResourceManager::Clear()
{
	//delete all shaders
	for (auto iterator : Shaders)
		glDeleteProgram(iterator.second.id);
	
	for (auto iterator : Textures)
		glDeleteTextures(1, &iterator.second.id);

    for (auto iterator : Cubemaps)
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
        texture.Generate(width, height, data);
    }
    else
        std::cout << "Texture failed to load at path: " << file << std::endl;
    
    stbi_image_free(data);

    return texture;
}

Cubemap ResourceManager::loadCubemapFromFileVector(std::vector<std::string> faces)
{
    Cubemap cubemap;
    glGenTextures(1, &cubemap.id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.id);
    int width, height, nrChannels;

    for (unsigned int i = 0; i < faces.size(); i++)
    {
       unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return cubemap;
}