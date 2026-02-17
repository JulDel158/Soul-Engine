#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Game.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include "Input/InputManager.hpp"

#include <iostream>

#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"

namespace 
{
    void Framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }
}

int main(int argc, char *argv[])
{
    ResourceManager& resourceManager = ResourceManager::GetInstance();
    Settings settings = resourceManager.LoadSettings();
    Game gameInstance = Game(settings);
    InputManager& inputManager = InputManager::GetInstance();
    inputManager.SetGameInstance(gameInstance);
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);
    GLFWwindow* window = glfwCreateWindow(settings.screen_width_, settings.screen_height_, "Soul", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    
    // load all OpenGL function pointers (glad)
    if (gladLoadGL(glfwGetProcAddress) == 0)
    {
        std::cout << "Failed to initialize OpenGL context\n" << std::endl;
        resourceManager.Clear();
        glfwTerminate();
        return -1;
    }
    
    //glfwSetKeyCallback(window, &InputManager::GetInstance().KeyCallback);
    glfwSetFramebufferSizeCallback(window, reinterpret_cast<GLFWframebuffersizefun>(Framebuffer_size_callback));
    
    // OpenGL configuration
    glViewport(0, 0, settings.screen_width_, settings.screen_height_);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // initialize game
    gameInstance.Init();
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();
        
        gameInstance.ProcessInput(deltaTime);

        // update game state and audio
        gameInstance.Update(deltaTime);
        gameInstance.ProcessAudio(deltaTime);

        // render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        gameInstance.Render(deltaTime);

        glfwSwapBuffers(window);
    }
    
    resourceManager.Clear();
    gameInstance.End();
    glfwTerminate();
    return 0;
}