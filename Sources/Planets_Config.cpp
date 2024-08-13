#include "../Headers/planets_config.h"
#include "../Headers/resource_manager.h"

void PlanetsConfig::LightingConfig(Shader shader, Light light)
{
    shader.SetVec3f("SunLight.position", light.position);
    shader.SetVec3f("SunLight.direction", light.direction);
    shader.SetVec3f("SunLight.diffuse", glm::vec3(light.diffuse));
    shader.SetVec3f("SunLight.ambient", glm::vec3(light.ambient));
    shader.SetVec3f("SunLight.specular", light.specular);

    //attenuation properties
    shader.SetFloat("SunLight.constant", light.constant);
    shader.SetFloat("SunLight.linear", light.linear);
    shader.SetFloat("SunLight.quadratic", light.quadratic);

}

void PlanetsConfig::MaterialConfig(Shader shader, Material mtl)
{
    shader.SetInt("PlanetMtl.diffuse", 0);
    shader.SetInt("PlanetMtl.specular", 1);
    shader.SetFloat("PlanetMtl.shineFactor", mtl.shineFact);
}

void PlanetsConfig::EarthConfig(Shader shader, glm::vec3 position, glm::vec3 scale, Texture2D nightMap, Texture2D cloudMap)
{
    shader.Use();
    shader.SetVec3f("SunLight.ambient", glm::vec3(0.0));
    glm::mat4 model = glm::mat4(1.0);
    shader.SetMat4("model", model);

    //earth additional textures
    shader.SetInt("PlanetMtl.textureLayer1", 6);
    glActiveTexture(GL_TEXTURE6);
    nightMap.Bind();


    shader.SetInt("PlanetMtl.textureLayer2", 7);
    glActiveTexture(GL_TEXTURE7);
    cloudMap.Bind();
}

void PlanetsConfig::PlanetConfig(Shader shader, glm::vec3 position, glm::vec3 scale)
{
    shader.Use();
    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    shader.SetMat4("model", model);
}

unsigned int PlanetsConfig::AtmosphereInitialize()
{
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };
    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return VAO;
}

void PlanetsConfig::DrawAtmosphere(Shader shader, unsigned int vao)
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}