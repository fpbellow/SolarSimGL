
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Headers/resource_manager.h"
#include "Headers/camera.h"
#include "Headers/model.h"
#include "Headers/system_config.h"
#include "Headers/frame_buffer.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

//camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float fcoef = 1000.0;

//timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool toggleHud = false;

int main()
{
    // glfw initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_SAMPLES, 4);


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SolarSim", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetKeyCallback(window, key_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
    

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    // configure global opengl state
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    /////////////////
    //hdr framebuffer
    unsigned int hdrFbo;
    glGenFramebuffers(1, &hdrFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFbo);

    unsigned int hdrColorBuffers[2];
    glGenTextures(2, hdrColorBuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, hdrColorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, hdrColorBuffers[i], 0);
    }

    //render buffer
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    //color attachment specification
    unsigned int colorAttachs[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, colorAttachs);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete" << std::endl;


    unsigned int blurFBO[2];
    unsigned int blurColorBuffers[2];
    glGenFramebuffers(2, blurFBO);
    glGenTextures(2, blurColorBuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, blurFBO[i]);
        glBindTexture(GL_TEXTURE_2D, blurColorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurColorBuffers[i], 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete" << std::endl;
    }

    //////////////////////

    //shader programs
    ResourceManager::LoadShader("Shaders/planets.vert", "Shaders/planets.frag", nullptr, "planetShade");
    Shader planetShader = ResourceManager::GetShader("planetShade");

    ResourceManager::LoadShader("Shaders/sun.vert", "Shaders/sun.frag", nullptr, "sunShade");
    Shader sunShader = ResourceManager::GetShader("sunShade");

    ResourceManager::LoadShader("Shaders/atmosphere.vert", "Shaders/atmosphere.frag", nullptr, "atmoShade");
    Shader atmoShader = ResourceManager::GetShader("atmoShade");

    ResourceManager::LoadShader("Shaders/skybox.vert", "Shaders/skybox.frag", nullptr, "galaxyShade");
    Shader galaxyShader = ResourceManager::GetShader("galaxyShade");

    ResourceManager::LoadShader("Shaders/screen.vert", "Shaders/screen.frag", nullptr, "screenShade");
    Shader screenShader = ResourceManager::GetShader("screenShade");

    ResourceManager::LoadShader("Shaders/gaussian_blur.vert", "Shaders/gaussian_blur.frag", nullptr, "blurShade");
    Shader blurShader = ResourceManager::GetShader("blurShade");


    //skybox configuration and texture
    Galaxy galaxy = SystemConfig::GalaxyConfig(galaxyShader);

    ResourceManager::LoadCubemap(galaxy.faces, "skybox");
    galaxy.skybox = ResourceManager::GetCubemap("skybox");



    //earth additional textures
    ResourceManager::LoadTexture("Assets/objects/earth/8k_earth_nightmap.jpg", "EarthNight");
    Texture2D earthNight = ResourceManager::GetTexture("EarthNight");

    ResourceManager::LoadTexture("Assets/objects/earth/8k_earth_clouds.jpg", "EarthClouds");
    Texture2D earthClouds = ResourceManager::GetTexture("EarthClouds");



    Material objectMat;
    objectMat.shineFact = 32.0f;

    Light sunLight;
    sunLight.position = glm::vec3(80.0, 2.0, 2.0);
    sunLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    sunLight.exposue = 3.65;

    sunLight.ambient = 0.05f;
    sunLight.diffuse = 0.5f;
    sunLight.specular = glm::vec3(1.0f);;

    sunLight.constant = 1.0f;
    sunLight.linear = 0.007f;
    sunLight.quadratic = 0.0002f;


    
    Model earth("Assets/objects/earth/earth.obj");
    Model sun("Assets/objects/sun/sun.obj");

    blurShader.Use();
    blurShader.SetInt("image", 0);

    screenShader.Use();
    screenShader.SetInt("screenTexture", 0);
    screenShader.SetInt("bloomBlur", 1);


    // main loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        if (camera.LockCursor)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindFramebuffer(GL_FRAMEBUFFER, hdrFbo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //vertex matrices
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, fcoef);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0);


        //shader setup
        planetShader.Use();

        planetShader.SetMat4("projection", projection);
        planetShader.SetMat4("view", view);

        planetShader.SetVec3f("viewPos", camera.Position);

        //lighting properties
        SystemConfig::LightingConfig(planetShader, sunLight);

        // material properties
        SystemConfig::MaterialConfig(planetShader, objectMat);
   
   
        //earth
        planetShader.SetBool("PlanetMtl.earth", true);
        planetShader.SetFloat("u_time", static_cast<float>(glfwGetTime()));
        SystemConfig::EarthConfig(planetShader, glm::vec3(0.0), glm::vec3(1.0), earthNight, earthClouds, static_cast<float>(glfwGetTime()));

        earth.Draw(planetShader);
        planetShader.SetBool("PlanetMtl.earth", false);

        //earth atmosphere
        atmoShader.Use();
        atmoShader.SetMat4("projection", projection);
        atmoShader.SetMat4("view", view);

        SystemConfig::LightingConfig(atmoShader, sunLight);
        SystemConfig::MaterialConfig(atmoShader, objectMat);
        SystemConfig::PlanetConfig(atmoShader, glm::vec3(0.0f), glm::vec3(1.01f));
        atmoShader.SetVec3f("viewPos", camera.Position);

        glEnable(GL_BLEND);
        earth.Draw(atmoShader);
        glDisable(GL_BLEND);


        ////venus
        //SystemConfig::PlanetConfig(planetShader, glm::vec3(1.0, 0.0, 0.0), glm::vec3(1.0));
        //venus.Draw(planetShader);

        //sun
        sunShader.Use();
        sunShader.SetMat4("projection", projection);
        sunShader.SetMat4("view", view);

        sunShader.SetVec3f("sunColor", glm::vec3(6.0f));
        SystemConfig::PlanetConfig(sunShader, sunLight.position, glm::vec3(0.025f));
        sun.Draw(sunShader);

        //draw skybox last
        glDepthFunc(GL_LEQUAL);
        galaxyShader.Use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        galaxyShader.SetMat4("view", view);
        galaxyShader.SetMat4("projection", projection);
        SystemConfig::GalaxyDraw(galaxy.vao, galaxy.skybox);
        glDepthFunc(GL_LESS);


        unsigned int screenVAO = 0;
        glGenVertexArrays(1, &screenVAO); 

        //blur bright fragments with gaussian
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glBindVertexArray(screenVAO); 
        SystemConfig::ConfigQuad(screenVAO); 
        bool horizontal = true, first_iter = true;
        unsigned int amount = 6;
        blurShader.Use();
        for (unsigned int i = 0; i < amount; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, blurFBO[horizontal]);
            blurShader.SetInt("horizontal", horizontal);
            glBindTexture(GL_TEXTURE_2D, first_iter ? hdrColorBuffers[1] : blurColorBuffers[!horizontal]);
            SystemConfig::RenderQuad(screenVAO);
            horizontal = !horizontal;
            if (first_iter)
                first_iter = false;
        }


        //draw postprocess quad

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(screenVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hdrColorBuffers[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, blurColorBuffers[0]);
        screenShader.Use();
        screenShader.SetFloat("exposure", sunLight.exposue);
        SystemConfig::RenderQuad(screenVAO);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //deallocate resources
    glDeleteVertexArrays(1, &galaxy.vao);
    ResourceManager::Clear();

    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        toggleHud = !toggleHud;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);


    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
        camera.LockCursor = true;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE)
        camera.LockCursor = false;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.enableZoom = true;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
        camera.enableZoom = false;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

}

void mouse_callback(GLFWwindow* window, double xposin, double yposin)
{
    float xpos = static_cast<float>(xposin);
    float ypos = static_cast<float>(yposin);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;
    
    if (camera.LockCursor)
    {
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{   
    if (camera.enableZoom)
    {
        camera.ProcessMouseScroll(static_cast<float>(yoffset));
    }
}