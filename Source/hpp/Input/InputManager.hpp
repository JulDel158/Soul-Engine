#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP
#pragma once

#include "glad/gl.h" // NOLINT
#include "GLFW/glfw3.h"
#include "glm/ext/vector_float2.hpp"
#include "robin_hood_hash/robin_hood.h"

#include "InputAction.hpp"

class InputManager
{
private:
    // NOTE: The map's key is the scancode of the pressed keyboard key. 
    // To retrieve a key's scancode call glfwGetKeyScancode(int keyValue) 
    // Where keyValue is the Keyboard value as declared in glfw3.h. Ex: GLFW_KEY_A, GLFW_KEY_SEMICOLON, GLFW_KEY_ESCAPE, etc.
    robin_hood::unordered_map<int, robin_hood::unordered_set<InputAction*>> keys_input_actions_;
    robin_hood::unordered_set<InputAction*> cursor_position_input_actions_;
    // NOTE: The map's key is the mouse button as defined in glfw3.h. Ex: GLFW_MOUSE_BUTTON_(1-8)
    robin_hood::unordered_map<int, robin_hood::unordered_set<InputAction*>> mouse_button_input_actions_;
    robin_hood::unordered_set<InputAction*> scroll_wheel_input_action_;
    // We will store a map for each gamepad/joystick supported
    robin_hood::unordered_map<int, robin_hood::unordered_set<InputAction*>> gamepad_button_input_actions_[GLFW_JOYSTICK_MAX_COUNT];
    robin_hood::unordered_map<int, robin_hood::unordered_set<InputAction*>> gamepad_axes_input_actions_[GLFW_JOYSTICK_MAX_COUNT];
    // For gamepads, we will have to poll the state, and manually send Pressed, Release events by comparing states
    GLFWgamepadstate current_gamepad_states_[GLFW_JOYSTICK_MAX_COUNT];
    GLFWgamepadstate previous_gamepad_states_[GLFW_JOYSTICK_MAX_COUNT];
    glm::vec2 previous_cursor_position_;
    GLFWwindow* window_;
    
    InputManager();
    ~InputManager();
    
public:
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;
    
    static InputManager& Instance();
    
    // ---- Callbacks ----
    static void InitializeInputManager(GLFWwindow* window);
    static void KeyboardButtonEventCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);
    static void CursorPositionEventCallback(GLFWwindow* window, double xPos, double yPos);
    static void MouseButtonEventCallback(GLFWwindow* window, int button, int action, int mods);
    static void ScrollWheelEventCallback(GLFWwindow* window, double xOffset, double yOffset);
    
    // For the gamepad, we must poll state changes instead
    void RetrieveAndProcessGamepadState();
    
    void InputUpdate(const float dt) const;
    
    // ---- Binding functions ----
    void BindInputAction(InputAction* const action, const int scancode = GLFW_KEY_UNKNOWN, const int controller = GLFW_JOYSTICK_1);
    void UnbindInputAction(InputAction* const action, const int scancode = GLFW_KEY_UNKNOWN, const int controller = GLFW_JOYSTICK_1);
    void UnbindInputActions();
    
    // ---- Configurations ----
    void ChangeCursorMode(const int mode) const;
    void SetRawMouseMotion(const bool mode) const;
    
private:
    
    // ---- Event processing ----
    void ProcessKeyboardButtonEvent(const int key, const int scancode, const int action, const int mods);
    void ProcessCursorEvent(const double xPos, const double yPos);
    void ProcessMouseButtonEvent(const int button, const int action, const int mods);
    void ProcessScrollWheelEvent(const double xOffset, const double yOffset) const;
};
#endif