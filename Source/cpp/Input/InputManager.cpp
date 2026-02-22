#include "Input/InputManager.hpp"

InputManager& InputManager::Instance()
{
    static InputManager instance = InputManager();
    return instance;
}

void InputManager::SetKeyCallback(GLFWwindow* window)
{
    glfwSetKeyCallback(window, &InputManager::KeyCallback);
}

void InputManager::KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
    Instance().InternalKeyCallback(window, key, scanCode, action, mods);
}

void InputManager::InputUpdate(const float dt) const
{
    for (const auto& inputActionPair : input_actions_) // NOLINT
    {
        for (auto& inputAction : inputActionPair.second)
        {
            inputAction->Update(dt);
        }
    }
}

void InputManager::BindInputAction(int scancode, InputAction* const action)
{
    if (action == nullptr)
    {
        return;
    }
    
    input_actions_[scancode].push_back(action);
}

InputManager::InputManager()
{
    
}

InputManager::~InputManager()
{
    
}

void InputManager::InternalKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // here we can communicate with the game object and send events and what not
    // TODO: this line is temporary
    // if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    // {
    //     glfwSetWindowShouldClose(window, GLFW_TRUE);
    // }
    
    if (input_actions_.contains(scancode))
    {
        switch (action)
        {
        case GLFW_PRESS:
            {
                for (const auto& inputAction : input_actions_[scancode])
                {
                    inputAction->Pressed();
                }
                break;
            }
        case GLFW_RELEASE:
            {
                for (const auto& inputAction : input_actions_[scancode])
                {
                    inputAction->Released();
                }
                break;
            }
        default:
            {
                break;
            }
        }
    }
}
