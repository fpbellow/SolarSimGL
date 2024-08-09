
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



#include <iostream>

#include "Headers/resource_manager.h"
#include "Headers/camera.h"
#include "Headers/model.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

struct Material {
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

//default colors
glm::vec3 lightColor = glm::vec3(1.0f);

bool toggleHud = false;

int main()
{
    // glfw initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


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

    ResourceManager::LoadShader("Shaders/planets.vert", "Shaders/planets.frag", nullptr, "planetShade");
    Shader planetShader = ResourceManager::GetShader("planetShade");

    ResourceManager::LoadShader("Shaders/sun.vert", "Shaders/sun.frag", nullptr, "sunShade");
    Shader sunShader = ResourceManager::GetShader("sunShade");


    //earth additional textures
    ResourceManager::LoadTexture("Assets/objects/earth/8k_earth_nightmap.jpg", "EarthNight");
    Texture2D earthNight = ResourceManager::GetTexture("EarthNight");

    ResourceManager::LoadTexture("Assets/objects/earth/8k_earth_clouds.jpg", "EarthClouds");
    Texture2D earthClouds = ResourceManager::GetTexture("EarthClouds");

    

    Material objectMat;
    objectMat.shineFact = 64.0f;

    Light sunLight;
    sunLight.position = glm::vec3(40.0, 2.0, 2.0);
    sunLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);

    sunLight.ambient = 0.1f;
    sunLight.diffuse = 0.5f;
    sunLight.specular = lightColor;

    sunLight.constant = 1.0f;
    sunLight.linear = 0.007f;
    sunLight.quadratic = 0.0002f;


    Model earth("Assets/objects/earth/earth.obj");
    Model venus("Assets/objects/venus/venus.obj");
    Model sun("Assets/objects/sun/sun.obj");


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


        planetShader.Use();
        planetShader.SetVec3f("SunLight.position", sunLight.position);
        
        planetShader.SetVec3f("SunLight.direction", sunLight.direction);
        planetShader.SetVec3f("viewPos", camera.Position);


        planetShader.SetVec3f("SunLight.ambient", glm::vec3(sunLight.ambient));
        planetShader.SetVec3f("SunLight.diffuse", glm::vec3(sunLight.diffuse));
        planetShader.SetVec3f("SunLight.specular", sunLight.specular);

        //attenuation properties
        planetShader.SetFloat("SunLight.constant", sunLight.constant);
        planetShader.SetFloat("SunLight.linear", sunLight.linear);
        planetShader.SetFloat("SunLight.quadratic", sunLight.quadratic);


        // material properties
        planetShader.SetInt("PlanetMtl.diffuse", 0);
        planetShader.SetInt("PlanetMtl.specular", 1); 
        planetShader.SetFloat("PlanetMtl.shineFactor", objectMat.shineFact);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, fcoef);

        glm::mat4 view = camera.GetViewMatrix();
        
        planetShader.SetMat4("projection", projection);
        planetShader.SetMat4("view", view);
        
        //earth
        glm::mat4 model = glm::mat4(1.0);
  
        model = glm::rotate(model, static_cast<float>(glfwGetTime()) * glm::radians(20.0f), glm::vec3(0.0, 1.0, 0.0));
        planetShader.SetMat4("model", model);
        planetShader.SetBool("PlanetMtl.earth", true);

        //earth additional textures
        planetShader.SetInt("PlanetMtl.textureLayer1", 6);
        glActiveTexture(GL_TEXTURE6);
        earthNight.Bind();


        planetShader.SetInt("PlanetMtl.textureLayer2", 7);
        glActiveTexture(GL_TEXTURE7);
        earthClouds.Bind();

        earth.Draw(planetShader);
        planetShader.SetBool("PlanetMtl.earth", false);

        //venus
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(1.0, 0.0, 0.0));
        planetShader.SetMat4("model", model);
        venus.Draw(planetShader);

        //sun
        sunShader.Use();
        sunShader.SetMat4("projection", projection);
        sunShader.SetMat4("view", view);

        model = glm::mat4(1.0);
        model = glm::translate(model, sunLight.position);
        model = glm::scale(model, glm::vec3(0.25f)); 
        sunShader.SetMat4("model", model);
        sun.Draw(sunShader);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
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