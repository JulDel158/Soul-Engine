#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Game.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include "Input/InputManager.hpp"
#include "Audio/AudioManager.hpp"

#include <iostream>

namespace 
{
    void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }
    
    auto ErrorCallback(int error, const char* description) -> void
    {
        static_cast<void>(fprintf(stderr, "Error: %s\n", description));
    }
}

int main(int argc, char *argv[])
{
    glfwSetErrorCallback(ErrorCallback);
    
    ResourceManager& resourceManager = ResourceManager::Instance();
    // Load and copy settings to initialize the rest of the program
    Settings settings;
    resourceManager.LoadSettings(settings);
    
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
        glfwTerminate();
        return -1;
    }
    
    if (settings.vsync_)
    {
        glfwSwapInterval(1);
    }
    
    glfwSetFramebufferSizeCallback(window, reinterpret_cast<GLFWframebuffersizefun>(FramebufferSizeCallback));
    
    // OpenGL configuration
    glViewport(0, 0, settings.screen_width_, settings.screen_height_);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Create and initialize game instance and input manager
    Game gameInstance = Game(settings);
    gameInstance.SetWindowPointer(window);
    InputManager& inputManager = InputManager::Instance();
    InputManager::InitializeInputManager(window);
    AudioManager& audioManager = AudioManager::Instance();
    
    float lastFrame = 0.0f;
    bool tempAudioFlag = true;

    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time
        const float currentFrame = static_cast<float>(glfwGetTime());
        const float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();
        inputManager.RetrieveAndProcessGamepadState();
        
        gameInstance.ProcessInput(deltaTime);
        
        // update game state and audio
        gameInstance.Update(deltaTime);
        //gameInstance.ProcessAudio(deltaTime);
        if (tempAudioFlag)
        {
            audioManager.PlaySound(0);
            tempAudioFlag = false;
        }
        
        // render
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        gameInstance.Render(deltaTime);
        
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    return 0;
}