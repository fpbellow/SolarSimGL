#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "texture.h"
#include "cubemap.h"

#include <string>


struct Material
{
    float shineFact;
};

struct Light 
{
    glm::vec3 position;
    glm::vec3 direction;

    float exposue;
    float ambient;
    float diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutoff;
    float outerCutoff;
};

struct Galaxy 
{
    std::vector<std::string> faces;
    Cubemap skybox;
    unsigned int vao;
};

class SystemConfig
{
public:

    //shader config
    void static LightingConfig(Shader shader, Light light);
    void static MaterialConfig(Shader shader, Material mtl);

    //planets
    void static EarthConfig(Shader shader, glm::vec3 position, glm::vec3 scale, Texture2D nightMap, Texture2D cloudMap, float time);
    void static PlanetConfig(Shader shader, glm::vec3 position, glm::vec3 scale);

    //galaxy config
    Galaxy static GalaxyConfig(Shader shader);
    void static GalaxyDraw(unsigned int galaxyVao, Cubemap skybox);

    //screen quad
    void static ConfigQuad(unsigned int quadVAO);
    void static RenderQuad(unsigned int quadVAO);

private:
    SystemConfig() {}
};

#endif
