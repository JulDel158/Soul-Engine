#include "Input/InputManager.hpp"

#include <iostream>

InputManager::InputManager() :
current_gamepad_states_{},
previous_gamepad_states_{},
previous_cursor_position_(glm::vec2(0.0f))
{
    cursor_position_input_actions_.reserve(2);
}

InputManager::~InputManager()
{
    UnbindInputActions();
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

void InputManager::RetrieveGamepadState()
{
    for (int gamepadId = 0; gamepadId < GLFW_JOYSTICK_LAST + 1; ++gamepadId)
    {
        GLFWgamepadstate state;
        if (glfwJoystickPresent(gamepadId) == GLFW_TRUE && glfwGetGamepadState(gamepadId, &state))
        {
            previous_gamepad_states_[gamepadId] = current_gamepad_states_[gamepadId]; 
            current_gamepad_states_[gamepadId] = state;
            
            // Button event handling
            for (int gamepadButton = 0; gamepadButton < GLFW_GAMEPAD_BUTTON_LAST + 1; ++gamepadButton)
            {
                const bool pressed = previous_gamepad_states_[gamepadId].buttons[gamepadButton] == GLFW_RELEASE && state.buttons[gamepadButton] == GLFW_PRESS;
                const bool released = previous_gamepad_states_[gamepadId].buttons[gamepadButton] == GLFW_PRESS && state.buttons[gamepadButton] == GLFW_RELEASE;
                
                for (auto& inputAction : gamepad_button_input_actions_[gamepadId][gamepadButton])
                {
                    if (pressed)
                    {
                        constexpr auto data = glm::vec2(1.0f);
                        inputAction->data_ = data;
                        inputAction->Pressed();
                    }
                    else if (released)
                    {
                        constexpr auto data = glm::vec2(0.0f);
                        inputAction->data_ = data;
                        inputAction->Released();
                    }
                }
            }
            
            // Axes event handling
            for (int gamepadAxes = 0; gamepadAxes < GLFW_GAMEPAD_AXIS_LAST + 1; ++gamepadAxes)
            {
                for (auto& inputAction : gamepad_axes_input_actions_[gamepadId][gamepadAxes])
                {
                    const float data = (glm::abs(state.axes[gamepadAxes]) >= inputAction->GetDeadZone()) ? state.axes[gamepadAxes] : 0.f;
                    const float triggerState = (state.axes[gamepadAxes] + 1.0f) / 2.0f; // state goes from -1 to 1, so we add 1 (0 to 2) and then divide it by 2 to make the range go from 0 to 1 instead
                    const float triggerData = triggerState >= inputAction->GetDeadZone() ? triggerState : 0.0f;
                    bool canUpdate = static_cast<bool>(data);
                    if (gamepadAxes == GLFW_GAMEPAD_AXIS_LEFT_Y || gamepadAxes == GLFW_GAMEPAD_AXIS_RIGHT_Y)
                    {
                        // place data on y
                        inputAction->data_.y = data;
                    }
                    else if (gamepadAxes == GLFW_GAMEPAD_AXIS_LEFT_TRIGGER || gamepadAxes == GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER)
                    {
                        // place data on x
                        inputAction->data_.x = triggerData;
                        canUpdate = static_cast<bool>(triggerData);
                    }
                    else
                    {
                        // place data on x
                        inputAction->data_.x = data;
                    }
                    
                    inputAction->SetCanUpdate(canUpdate);
                }
            }
        }
    }
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
    
    for (auto& map : gamepad_button_input_actions_)
    {
        for (auto& pair : map)
        {
            for (auto& inputAction : pair.second)
            {
                inputAction->Update(dt);
            }
        }
    }
    
    for (auto& map : gamepad_axes_input_actions_)
    {
        for (auto& pair : map)
        {
            for (auto& inputAction : pair.second)
            {
                inputAction->Update(dt);
                inputAction->SetCanUpdate(false);
            }
        }
    }
}

void InputManager::BindInputAction(InputAction* const action, const int scancode, const int controller)
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
    case EInputActionType::Gamepad_Button:
        {
            if (scancode >= 0 && scancode <= GLFW_GAMEPAD_BUTTON_LAST && controller >= GLFW_JOYSTICK_1 && controller <= GLFW_JOYSTICK_LAST)
            {
                gamepad_button_input_actions_[controller][scancode].insert(action);
            }
            break;
        }
    case EInputActionType::Gamepad_Axes:
        {
            if (scancode >= 0 && scancode <= GLFW_GAMEPAD_AXIS_LAST && controller >= GLFW_JOYSTICK_1 && controller <= GLFW_JOYSTICK_LAST)
            {
                gamepad_axes_input_actions_[controller][scancode].insert(action);
            }
            break;
        }
    }
}

void InputManager::UnbindInputAction(InputAction* const action, const int scancode, const int controller)
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
    case EInputActionType::Gamepad_Button:
        {
            if (scancode >= 0 && scancode <= GLFW_GAMEPAD_BUTTON_LAST && controller >= GLFW_JOYSTICK_1 && controller <= GLFW_JOYSTICK_LAST)
            {
                gamepad_button_input_actions_[controller][scancode].erase(action);
            }
            break;
        }
    case EInputActionType::Gamepad_Axes:
        {
            if (scancode >= 0 && scancode <= GLFW_GAMEPAD_AXIS_LAST && controller >= GLFW_JOYSTICK_1 && controller <= GLFW_JOYSTICK_LAST)
            {
                gamepad_axes_input_actions_[controller][scancode].erase(action);
            }
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
    
    for (auto& map : gamepad_button_input_actions_)
    {
        map.clear();
    }
    
    for (auto& map : gamepad_axes_input_actions_)
    {
        map.clear();
    }
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