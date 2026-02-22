#include "Input/InputManager.hpp"

#include <iostream>
#include <ostream>

InputManager::InputManager()
{
    // TODO: Init data
}

InputManager::~InputManager()
{
    // TODO: Clear data
}

InputManager& InputManager::Instance()
{
    static InputManager instance = InputManager();
    return instance;
}

void InputManager::SetKeyCallback(GLFWwindow* window)
{
    glfwSetKeyCallback(window, &InputManager::KeyCallback);
    glfwSetCursorPosCallback(window, &InputManager::CursorPositionCallback);
}

void InputManager::KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
    Instance().InternalKeyCallback(window, key, scanCode, action, mods);
}

void InputManager::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    Instance().InternalCursorPositionCallback(window, xpos, ypos);
}

void InputManager::InputUpdate(const float dt) const
{
    for (const auto& inputActionPair : keys_input_actions_) // NOLINT
    {
        for (auto& inputAction : inputActionPair.second)
        {
            inputAction->Update(dt);
        }
    }
    
    for (const auto& inputAction : cursor_position_input_actions_)
    {
        inputAction->Update(dt);
        inputAction->SetCanUpdate(false);
    }
}

void InputManager::BindInputAction(InputAction* const action, int scancode)
{
    if (action == nullptr)
    {
        return;
    }
    switch (action->GetType())
    {
    case EInputActionType::None:
        {
            std::cout << "WARNING::BindInputAction: The input action had no type!!!!" << std::endl;
            break;
        }
    case EInputActionType::Keyboard:
        {
            keys_input_actions_[scancode].push_back(action);
            break;
        }
    case EInputActionType::Mouse_Cursor:
        {
            cursor_position_input_actions_.push_back(action);
            break;
        }
    case EInputActionType::Mouse_Button:
        {
            //TODO: store pointer to this type of input action
            break;
        }
    case EInputActionType::Mouse_Wheel:
        {
            //TODO: store pointer to this type of input action
            break;
        }
    case EInputActionType::Gamepad:
        {
            //TODO: store pointer to this type of input action
            break;
        }
    }
    
}

void InputManager::InternalKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // here we can communicate with the game object and send events and what not
    // TODO: this line is temporary
    // if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    // {
    //     glfwSetWindowShouldClose(window, GLFW_TRUE);
    // }
    
    if (keys_input_actions_.contains(scancode))
    {
        switch (action)
        {
        case GLFW_PRESS:
            {
                for (const auto& inputAction : keys_input_actions_[scancode])
                {
                    inputAction->Pressed();
                }
                break;
            }
        case GLFW_RELEASE:
            {
                for (const auto& inputAction : keys_input_actions_[scancode])
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

void InputManager::InternalCursorPositionCallback(const GLFWwindow* const window, const double x, const double y)
{
    const glm::vec2 position(static_cast<float>(x), static_cast<float>(y));
    for (auto& inputAction : cursor_position_input_actions_)
    {
        switch (inputAction->GetCursorDataMode())
        {
        case ECursorDataMode::Position:
            {
                inputAction->data_ = position;
                break;
            }
        case ECursorDataMode::Clamped_Additive:
            {
                inputAction->data_ = glm::clamp(inputAction->data_ + (previous_cursor_position_ - position),0.0f, inputAction->GetClampMax());
                break;
            }
        case ECursorDataMode::Direction:
            {
                inputAction->data_ = glm::normalize(previous_cursor_position_ - position);
                break;
            }
        }
        
        // We want to call update this frame during input update
        inputAction->SetCanUpdate(true);
    }
    
    previous_cursor_position_ = position;
}
