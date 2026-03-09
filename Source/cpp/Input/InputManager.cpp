#include "Input/InputManager.hpp"

#include "glm/ext/vector_common.hpp"
#include "glm/ext/quaternion_geometric.hpp"

#include "Utils/Logger.hpp"
#include "EngineDataStructures.hpp"

namespace {
    constexpr auto ZERO_VECTOR = glm::vec2(0.0f);
    constexpr auto ONE_VECTOR = glm::vec2(1.0f);
}

InputManager::InputManager() :
current_gamepad_states_{},
previous_gamepad_states_{},
previous_cursor_position_(ZERO_VECTOR),
window_(nullptr)
{
    cursor_position_input_actions_.reserve(2);
}

InputManager::~InputManager()
{
    UnbindInputActions();
    window_ = nullptr;
}

InputManager& InputManager::Instance()
{
    static InputManager instance = InputManager();
    return instance;
}

void InputManager::InitializeInputManager(GLFWwindow* window)
{
    Instance().window_ = window;
    glfwSetKeyCallback(window, &InputManager::KeyboardButtonEventCallback);
    glfwSetCursorPosCallback(window, &InputManager::CursorPositionEventCallback);
    glfwSetMouseButtonCallback(window, &InputManager::MouseButtonEventCallback);
    glfwSetScrollCallback(window, &InputManager::ScrollWheelEventCallback);
    
    //Instance().ChangeCursorMode(GLFW_CURSOR_DISABLED);
    // This could cause discrepancies in some devices, but preferred for control
    Instance().SetRawMouseMotion(false);
}

void InputManager::KeyboardButtonEventCallback(GLFWwindow* window, int key, int scanCode, int action, int mods) // NOLINT
{
    Instance().ProcessKeyboardButtonEvent(key, scanCode, action, mods);
}

void InputManager::CursorPositionEventCallback(GLFWwindow* window, double xPos, double yPos) // NOLINT
{
    Instance().ProcessCursorEvent(xPos, yPos);
}

void InputManager::MouseButtonEventCallback(GLFWwindow* window, int button, int action, int mods) // NOLINT
{
    Instance().ProcessMouseButtonEvent(button, action, mods);
}

void InputManager::ScrollWheelEventCallback(GLFWwindow* window, double xOffset, double yOffset) // NOLINT
{
    Instance().ProcessScrollWheelEvent(xOffset, yOffset);
}

void InputManager::RetrieveAndProcessGamepadState()
{
    for (int gamepadId = 0; gamepadId < GLFW_JOYSTICK_MAX_COUNT; ++gamepadId)
    {
        GLFWgamepadstate state;
        if (glfwJoystickPresent(gamepadId) == GLFW_TRUE && glfwGetGamepadState(gamepadId, &state))
        {
            previous_gamepad_states_[gamepadId] = current_gamepad_states_[gamepadId]; 
            current_gamepad_states_[gamepadId] = state;
            
            // Button event handling
            for (int gamepadButton = 0; gamepadButton < GLFW_GAMEPAD_BUTTON_MAX_COUNT; ++gamepadButton)
            {
                const bool pressed = previous_gamepad_states_[gamepadId].buttons[gamepadButton] == GLFW_RELEASE && state.buttons[gamepadButton] == GLFW_PRESS;
                const bool released = previous_gamepad_states_[gamepadId].buttons[gamepadButton] == GLFW_PRESS && state.buttons[gamepadButton] == GLFW_RELEASE;
                
                for (auto& inputAction : gamepad_button_input_actions_[gamepadId][gamepadButton])
                {
                    if (pressed)
                    {
                        inputAction->data_ = ONE_VECTOR;
                        inputAction->Pressed();
                    }
                    else if (released)
                    {
                        inputAction->data_ = ZERO_VECTOR;
                        inputAction->Released();
                    }
                }
            }
            
            // Axes event handling
            for (int gamepadAxes = 0; gamepadAxes < GLFW_GAMEPAD_AXIS_MAX_COUNT; ++gamepadAxes)
            {
                for (auto& inputAction : gamepad_axes_input_actions_[gamepadId][gamepadAxes])
                {
                    const float data = (glm::abs(state.axes[gamepadAxes]) >= inputAction->GetDeadZone()) ? state.axes[gamepadAxes] : 0.0f;
                    bool canUpdate = static_cast<bool>(data);
                    if (gamepadAxes == GLFW_GAMEPAD_AXIS_LEFT_Y || gamepadAxes == GLFW_GAMEPAD_AXIS_RIGHT_Y)
                    {
                        // place data on y
                        inputAction->data_.y = data;
                    }
                    else if (gamepadAxes == GLFW_GAMEPAD_AXIS_LEFT_TRIGGER || gamepadAxes == GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER)
                    {
                        const float triggerState = (state.axes[gamepadAxes] + 1.0f) / 2.0f; // state goes from -1 to 1, so we add 1 (0 to 2) and then divide it by 2 to make the range go from 0 to 1 instead
                        const float triggerData = triggerState >= inputAction->GetDeadZone() ? triggerState : 0.0f;
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
        for (auto& pair : map) // NOLINT
        {
            for (auto& inputAction : pair.second)
            {
                inputAction->Update(dt);
            }
        }
    }
    
    for (auto& map : gamepad_axes_input_actions_)
    {
        for (auto& pair : map) // NOLINT
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
    		auto logger = Logger();
            logger.Log(ELogLevel::Warning,"InputManager::BindInputAction: The input action had no type!!!!");
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
            else
            {
            	auto logger = Logger();
            	logger.Log(ELogLevel::Warning,
            		"InputManager::BindInputAction: The input action was Gamepad_Button but scancode: [" +
            		std::to_string(scancode) +
            		"] may be invalid or \n controller: [" +
            		std::to_string(controller) + "] was invalid!!");
            }
            break;
        }
    case EInputActionType::Gamepad_Axes:
        {
            if (scancode >= 0 && scancode <= GLFW_GAMEPAD_AXIS_LAST && controller >= GLFW_JOYSTICK_1 && controller <= GLFW_JOYSTICK_LAST)
            {
                gamepad_axes_input_actions_[controller][scancode].insert(action);
            }
            else
            {
            	auto logger = Logger();
            	logger.Log(ELogLevel::Warning,
					"InputManager::BindInputAction: The input action was Gamepad_Axes but scancode: [" +
					std::to_string(scancode) +
					"] may be invalid or \n controller: [" +
					std::to_string(controller) + "] was invalid!!");
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
    		auto logger = Logger();
    		logger.Log(ELogLevel::Warning,"InputManager::UnbindInputAction: The input action had no type!!!!");
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
    
    for (auto& gamepad : gamepad_button_input_actions_)
    {
        gamepad.clear();
    }
    
    for (auto& gamepad : gamepad_axes_input_actions_)
    {
        gamepad.clear();
    }
}

void InputManager::ChangeCursorMode(const int mode) const
{
    if (window_ == nullptr)
    {
        return;
    }
    glfwSetInputMode(window_, GLFW_CURSOR, mode);
}

void InputManager::SetRawMouseMotion(const bool mode) const
{
    if (window_ == nullptr)
    {
        return;
    }
    if (glfwRawMouseMotionSupported())
    {
        glfwSetInputMode(window_, GLFW_RAW_MOUSE_MOTION, mode);
    }
}

void InputManager::ProcessKeyboardButtonEvent(const int key, const int scancode, const int action, const int mods)
{
    if (keys_input_actions_.contains(scancode))
    {
        switch (action)
        {
        case GLFW_PRESS:
            {
                for (const auto& inputAction : keys_input_actions_[scancode])
                {
                    inputAction->data_ = ONE_VECTOR;
                    inputAction->Pressed();
                }
                break;
            }
        case GLFW_RELEASE:
            {
                for (const auto& inputAction : keys_input_actions_[scancode])
                {
                    inputAction->data_ = ZERO_VECTOR;
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

void InputManager::ProcessCursorEvent(const double xPos, const double yPos)
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

void InputManager::ProcessMouseButtonEvent(const int button, const int action, const int mods)
{
    switch (action)
    {
    case GLFW_PRESS:
        {
            for (const auto& inputAction : mouse_button_input_actions_[button])
            {
                inputAction->data_ = ONE_VECTOR;
                inputAction->Pressed();
            }
            break;
        }
    case GLFW_RELEASE:
        {
            for (const auto& inputAction : mouse_button_input_actions_[button])
            {
                inputAction->data_ = ZERO_VECTOR;
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

void InputManager::ProcessScrollWheelEvent(const double xOffset, const double yOffset) const
{
    const auto data = glm::vec2(static_cast<float>(xOffset), static_cast<float>(yOffset));
    for (auto& inputAction : scroll_wheel_input_action_)
    {
        inputAction->data_ = data;
        inputAction->SetCanUpdate(true);
    }
}