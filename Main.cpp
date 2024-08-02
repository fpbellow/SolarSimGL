
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>

#include "./Headers/resource_manager.h"
#include "./Headers/camera.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void mouse_callback(GLFWwindow* window, double xpos, double ypos );
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

//timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


//default colors
glm::vec3 lightColor = glm::vec3(1.0f);

bool toggleHud = false;
bool toggleLight = false;

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

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    // configure global opengl state
// -----------------------------
    glEnable(GL_DEPTH_TEST);

    ResourceManager::LoadShader("./Shaders/lighting.vert", "./Shaders/lighting.frag", nullptr, "lightShade");
    Shader lightShader = ResourceManager::GetShader("lightShade");

    ResourceManager::LoadShader("./Shaders/l_Cube.vert", "./Shaders/l_Cube.frag", nullptr, "lightCubeShade");
    Shader  lCubeShader = ResourceManager::GetShader("lightCubeShade");

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
    }; 

    Material objectMat;
    objectMat.shineFact = 32.0f;

    Light lcube;
    lcube.position = lightPos;
    lcube.direction = glm::vec3(-0.2f, -1.0f, -0.3f);

    lcube.ambient = 0.2f;
    lcube.diffuse = 0.5f;
    lcube.specular = lightColor;

    lcube.constant = 1.0f;
    lcube.linear = 0.07f;
    lcube.quadratic = 0.017f;

    lcube.cutoff = glm::cos(glm::radians(12.5));
    lcube.outerCutoff = glm::cos(glm::radians(17.5));


    //configure cube VAO & VBO
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    //position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //normal attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    //texture attributes
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //lighting VAO + VBO
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //lighting vertex attrib
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    ResourceManager::LoadTexture("./Assets/textures/container2.png", true, "containTex");
    Texture2D diffuseMap = ResourceManager::GetTexture("containTex");

    ResourceManager::LoadTexture("./Assets/textures/container2_specular.png", true, "contain_spec_Tex");
    Texture2D specularMap = ResourceManager::GetTexture("contain_spec_Tex");

    ResourceManager::LoadTexture("./Assets/textures/matrix.jpg", false, "matrix_em");
    Texture2D emissionMap = ResourceManager::GetTexture("matrix_em");





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




        lightShader.Use();
        lightShader.SetVec3f("FragLight.position", lcube.position);
        
        lightShader.SetVec3f("FragLight.direction", lcube.direction);
        lightShader.SetVec3f("viewPos", camera.Position);
        lightShader.SetFloat("u_time", glfwGetTime());

        
        lightShader.SetVec3f("FragLight.ambient", glm::vec3(lcube.ambient));
        lightShader.SetVec3f("FragLight.diffuse", glm::vec3(lcube.diffuse));
        lightShader.SetVec3f("FragLight.specular", lcube.specular);

        //attenuation properties
        lightShader.SetFloat("FragLight.constant", lcube.constant);
        lightShader.SetFloat("FragLight.linear", lcube.linear);
        lightShader.SetFloat("FragLight.quadratic", lcube.quadratic);

        //spotlight properties
        if (toggleLight)
        {
            lightShader.SetFloat("FragLight.cutoff", lcube.cutoff);
            lightShader.SetFloat("FragLight.outerCutoff", lcube.outerCutoff);
        }
        else
        {
            lightShader.SetFloat("FragLight.cutoff", 1.0);
            lightShader.SetFloat("FragLight.outerCutoff", 1.0);
        }
        
        // material properties
        lightShader.SetInt("FragMaterial.diffuse", 0);
        lightShader.SetInt("FragMaterial.specular", 1); 
        /*lightShader.SetInt("FragMaterial.emission", 2);*/
        lightShader.SetFloat("FragMaterial.shineFactor", objectMat.shineFact);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        glm::mat4 view = camera.GetViewMatrix();
        
        lightShader.SetMat4("projection", projection);
        lightShader.SetMat4("view", view);
        

        //world transform
        glm::mat4 model = glm::mat4(1.0);
        lightShader.SetMat4("model", model);

        //bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        diffuseMap.Bind();

        //bind specular map
        glActiveTexture(GL_TEXTURE1);
        specularMap.Bind();


        //render the cube
        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightShader.SetMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        

        //also draw the lamp object
        lCubeShader.Use();
        lCubeShader.SetVec3f("lampColor", lcube.specular + lcube.ambient);
        lCubeShader.SetMat4("projection", projection);
        lCubeShader.SetMat4("view", view);

        model = glm::mat4(1.0);
        model = glm::translate(model, lcube.position);
        model = glm::scale(model, glm::vec3(0.2f)); //scale down to a smaller cube
        lCubeShader.SetMat4("model", model);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }



    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);

    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        toggleHud = !toggleHud;

    if (key == GLFW_KEY_T && action == GLFW_PRESS)
        toggleLight = !toggleLight;
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
    {
        camera.LockCursor = true;
    }
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