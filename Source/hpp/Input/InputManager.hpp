#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "glad/gl.h" // NOLINT
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "InputAction.hpp"

#include <unordered_map>
#include <unordered_set>

//TODO: Handle keyboard input
//TODO: Handle mouse input
//TODO: Handle controller input

class InputManager
{
private:
    // NOTE: The map's key is the scancode of the pressed keyboard key. 
    // To retrieve a key's scancode call glfwGetKeyScancode(int keyValue) 
    // Where keyValue is the Keyboard value as declared in glfw3.h. Ex: GLFW_KEY_A, GLFW_KEY_SEMICOLON, GLFW_KEY_ESCAPE, etc.
    std::unordered_map<int, std::unordered_set<InputAction*>> keys_input_actions_;
    std::unordered_set<InputAction*> cursor_position_input_actions_;
    // NOTE: The map's key is the mouse button as defined in glfw3.h. Ex: GLFW_MOUSE_BUTTON_(1-8)
    std::unordered_map<int, std::unordered_set<InputAction*>> mouse_button_input_actions_;
    std::unordered_set<InputAction*> scroll_wheel_input_action_;
    glm::vec2 previous_cursor_position_;
    
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
    // TODO: add required callback for handling gamepad
    
    void InputUpdate(const float dt) const;
    
    // ---- Binding functions ----
    void BindInputAction(InputAction* const action, const int scancode = glfwGetKeyScancode(GLFW_KEY_UNKNOWN));
    void UnbindInputAction(InputAction* const action, const int scancode = glfwGetKeyScancode(GLFW_KEY_UNKNOWN));
    void UnbindInputActions();
    
private:
    InputManager();
    ~InputManager();
    
    // ---- Event processing ----
    void ProcessKeyboardButtonEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
    void ProcessCursorEvent(const GLFWwindow* const window, double xPos, double yPos);
    void ProcessMouseButtonEvent(GLFWwindow* window, const int button, const int action, const int mods);
    void ProcessScrollWheelEvent(GLFWwindow* window, double xOffset, double yOffset) const;
    // TODO: add function to process gamepad events
};

#endif
