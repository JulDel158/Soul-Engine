#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "Game.hpp"

//TODO: Handle keyboard and mouse input
//TODO: Handle controller input

class InputManager
{
public:
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;
    
    static InputManager& GetInstance();
    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void SetGameInstance(const Game& game);
    
private:
    
    Game* game_instance_;
    
    InputManager();
    ~InputManager();
    
};

#endif
