#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "glad/gl.h" // NOLINT
#include "GLFW/glfw3.h"

#include "Game.hpp"
#include "InputAction.hpp"

#include <unordered_map>
#include <vector>

//TODO: Handle keyboard input
//TODO: Handle mouse input
//TODO: Handle controller input

class InputManager
{
private:
    // NOTE: The map's key is the scancode of the key (keyboard) pressed
    std::unordered_map<int, std::vector<InputAction*>> input_actions_;
    
public:
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;
    
    static InputManager& Instance();
    static void SetKeyCallback(GLFWwindow* window);
    static void KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);
    void InputUpdate(const float dt) const;
    
    void BindInputAction(int scancode, InputAction* const action);
    
private:
    InputManager();
    ~InputManager();
    
    void InternalKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif
