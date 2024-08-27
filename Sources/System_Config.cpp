#include "../Headers/system_config.h"
#include "../Headers/resource_manager.h"

void SystemConfig::LightingConfig(Shader shader, Light light)
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

void SystemConfig::MaterialConfig(Shader shader, Material mtl)
{
    shader.SetInt("PlanetMtl.diffuse", 0);
    shader.SetInt("PlanetMtl.specular", 1);
    shader.SetFloat("PlanetMtl.shineFactor", mtl.shineFact);
}

void SystemConfig::EarthConfig(Shader shader, glm::vec3 position, glm::vec3 scale, Texture2D nightMap, Texture2D cloudMap, float time)
{
    shader.Use();
    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, position); 
    //model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //model = glm::rotate(model, glm::radians(1.0f) * (1.25f * time), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, scale);
    shader.SetMat4("model", model);

    //earth additional textures
    shader.SetInt("PlanetMtl.textureLayer1", 6);
    glActiveTexture(GL_TEXTURE6);
    nightMap.Bind();


    shader.SetInt("PlanetMtl.textureLayer2", 7);
    glActiveTexture(GL_TEXTURE7);
    cloudMap.Bind();
}

void SystemConfig::PlanetConfig(Shader shader, glm::vec3 position, glm::vec3 scale)
{
    shader.Use();
    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    shader.SetMat4("model", model);
}

Galaxy SystemConfig::GalaxyConfig(Shader shader)
{
    Galaxy galaxy;

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);

    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    std::vector<std::string> faces
    {
        std::string("Assets/textures/skybox/right.jpg"),
        std::string("Assets/textures/skybox/left.jpg"),
        std::string("Assets/textures/skybox/top.jpg"),
        std::string("Assets/textures/skybox/bottom.jpg"),
        std::string("Assets/textures/skybox/front.jpg"),
        std::string("Assets/textures/skybox/back.jpg")
    };

    galaxy.faces = faces;
    galaxy.vao = skyboxVAO;
    shader.Use();
    shader.SetInt("skybox", 0);

    return galaxy;
}

void SystemConfig::GalaxyDraw(unsigned int galaxyVao, Cubemap skybox)
{
    glBindVertexArray(galaxyVao);
    glActiveTexture(GL_TEXTURE0);
    skybox.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void SystemConfig::RenderQuad(unsigned int quadVAO)
{
    if (quadVAO == 0)
    {
        float quadVertices[] = { // vertex attributes for screen quad
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };

        //scren quad configuration
        unsigned int quadVBO;
        glGenBuffers(1, &quadVBO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}