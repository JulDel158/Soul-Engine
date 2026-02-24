#include "Input/InputManager.hpp"

#include <iostream>

InputManager::InputManager() :
previous_cursor_position_(glm::vec2(0.0f))
{
    cursor_position_input_actions_.reserve(2);
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

void InputManager::InitializeInputManager(GLFWwindow* window)
{
    glfwSetKeyCallback(window, &InputManager::KeyboardButtonEventCallback);
    glfwSetCursorPosCallback(window, &InputManager::CursorPositionEventCallback);
    glfwSetMouseButtonCallback(window, &InputManager::MouseButtonEventCallback);
    glfwSetScrollCallback(window, &InputManager::ScrollWheelEventCallback);
    //TODO: set gamepad callback
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // This could cause discrepancies in some devices, but preferred for control
    if (glfwRawMouseMotionSupported())
    {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
}

void InputManager::KeyboardButtonEventCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
    Instance().ProcessKeyboardButtonEvent(window, key, scanCode, action, mods);
}

void InputManager::CursorPositionEventCallback(GLFWwindow* window, double xPos, double yPos)
{
    Instance().ProcessCursorEvent(window, xPos, yPos);
}

void InputManager::MouseButtonEventCallback(GLFWwindow* window, int button, int action, int mods)
{
    Instance().ProcessMouseButtonEvent(window, button, action, mods);
}

void InputManager::ScrollWheelEventCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    Instance().ProcessScrollWheelEvent(window, xOffset, yOffset);
}

// TODO: Add gamepad event callback

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
    
    for (const auto& inputActionPair : mouse_button_input_actions_) //NOLINT
    {
        for (auto& inputAction : inputActionPair.second)
        {
            inputAction->Update(dt);
        }
    }

    for (auto& inputAction : scroll_wheel_input_action_)
    {
        inputAction->Update(dt);
        inputAction->SetCanUpdate(false);
    }
    
    // TODO: Update gamepad input actions
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
            keys_input_actions_[scancode].insert(action);
            break;
        }
    case EInputActionType::Mouse_Cursor:
        {
            cursor_position_input_actions_.insert(action);
            break;
        }
    case EInputActionType::Mouse_Button:
        {
            mouse_button_input_actions_[scancode].insert(action);
            break;
        }
    case EInputActionType::Mouse_Wheel:
        {
            scroll_wheel_input_action_.insert(action);
            break;
        }
    case EInputActionType::Gamepad:
        {
            //TODO: store pointer to this type of input action
            break;
        }
    }
}

void InputManager::UnbindInputAction(InputAction* const action, const int scancode)
{
    if (action == nullptr)
    {
        return;
    }
    
    switch (action->GetType())
    {
    case EInputActionType::None:
        {
            std::cout << "WARNING::UnbindInputAction: The input action had no type!!!!" << std::endl;
            break;
        }
    case EInputActionType::Keyboard:
        {
            if (keys_input_actions_.contains(scancode))
            {
                keys_input_actions_[scancode].erase(action);
            }
            break;
        }
    case EInputActionType::Mouse_Cursor:
        {
            cursor_position_input_actions_.erase(action);
            break;
        }
    case EInputActionType::Mouse_Button:
        {
            if (mouse_button_input_actions_.contains(scancode))
            {
                mouse_button_input_actions_[scancode].erase(action);
            }
            break;
        }
    case EInputActionType::Mouse_Wheel:
        {
            scroll_wheel_input_action_.erase(action);
            break;
        }
    case EInputActionType::Gamepad:
        {
            //TODO: unbind this input action
            break;
        }
    }
}

void InputManager::UnbindInputActions()
{
    keys_input_actions_.clear();
    cursor_position_input_actions_.clear();
    mouse_button_input_actions_.clear();
    scroll_wheel_input_action_.clear();
    //TODO: clear gamepad input actions
}

void InputManager::ProcessKeyboardButtonEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (keys_input_actions_.contains(scancode))
    {
        switch (action)
        {
        case GLFW_PRESS:
            {
                constexpr auto data = glm::vec2(1.0f);
                for (const auto& inputAction : keys_input_actions_[scancode])
                {
                    inputAction->data_ = data;
                    inputAction->Pressed();
                }
                break;
            }
        case GLFW_RELEASE:
            {
                constexpr auto data = glm::vec2(0.0f);
                for (const auto& inputAction : keys_input_actions_[scancode])
                {
                    inputAction->data_ = data;
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

void InputManager::ProcessCursorEvent(const GLFWwindow* const window, const double xPos, const double yPos)
{
    const glm::vec2 position(static_cast<float>(xPos), static_cast<float>(yPos));
    const glm::vec2 difference = previous_cursor_position_ - position;
    for (auto& inputAction : cursor_position_input_actions_)
    {
        switch (inputAction->GetCursorDataMode())
        {
        case ECursorDataMode::Position:
            {
                inputAction->data_ = position;
                break;
            }
        case ECursorDataMode::Direction:
            {
                inputAction->data_ = glm::vec2(
                    (difference.x > 0.0f) ? 1.0f : (difference.y < 0.0f) ? -1.0f : 0.0f,
                    (difference.y > 0.0f) ? 1.0f : (difference.y < 0.0f) ? -1.0f : 0.0f);
                break;
            }
        case ECursorDataMode::Normalized:
            {
                inputAction->data_ = glm::normalize(difference);
                break;
            }
        case ECursorDataMode::Additive:
            {
                inputAction->data_ += difference;
                break;
            }
        case ECursorDataMode::Additive_Clamped:
            {
                inputAction->data_ = glm::clamp(inputAction->data_ + difference,0.0f, inputAction->GetClampMax());
                break;
            }
        }
        
        // We want to call update this frame during input update
        inputAction->SetCanUpdate(true);
    }
    
    //std::cout << "X: " << x << " Y: " << y << std::endl;
    previous_cursor_position_ = position;
}

void InputManager::ProcessMouseButtonEvent(GLFWwindow* window, const int button, const int action, const int mods)
{
    switch (action)
    {
    case GLFW_PRESS:
        {
            constexpr auto data = glm::vec2(1.0f);
            for (const auto& inputAction : mouse_button_input_actions_[button])
            {
                inputAction->data_ = data;
                inputAction->Pressed();
            }
            break;
        }
    case GLFW_RELEASE:
        {
            constexpr auto data = glm::vec2(0.0f);
            for (const auto& inputAction : mouse_button_input_actions_[button])
            {
                inputAction->data_ = data;
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

void InputManager::ProcessScrollWheelEvent(GLFWwindow* window, double xOffset, double yOffset) const
{
    const auto data = glm::vec2(static_cast<float>(xOffset), static_cast<float>(yOffset));
    for (auto& inputAction : scroll_wheel_input_action_)
    {
        inputAction->data_ = data;
        inputAction->SetCanUpdate(true);
    }
}

// TODO: Define gamepad process event function