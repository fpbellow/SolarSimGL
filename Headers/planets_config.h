#ifndef PLANETS_CONFIG_H
#define PLANETS_CONFIG_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "texture.h"

#include <string>


struct Material{
    float shineFact;
};

struct Light {
    glm::vec3 position;
    glm::vec3 direction;

    float ambient;
    float diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutoff;
    float outerCutoff;
};

class PlanetsConfig
{
public:
    void static LightingConfig(Shader shader, Light light);
    void static MaterialConfig(Shader shader, Material mtl);

    //planets
    void static EarthConfig(Shader shader, glm::vec3 position, glm::vec3 scale, Texture2D nightMap, Texture2D cloudMap);
    void static PlanetConfig(Shader shader, glm::vec3 position, glm::vec3 scale);

private:
    PlanetsConfig() {}
};

#endif
