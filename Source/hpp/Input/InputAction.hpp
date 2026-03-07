#pragma once

#include "glm/ext/vector_float2.hpp"
#include "EngineDataStructures.hpp"
#include <functional>

class InputAction
{
public:
    glm::vec2 data_;
private:
    std::function<void(const glm::vec2&)> function_pressed_;
    std::function<void()> function_released_;
    std::function<void(const glm::vec2&, const float& deltaTime)> function_update_;
    float clamp_max_;
    float dead_zone_;
    EInputActionType type_;
    ECursorDataMode cursor_data_mode_;
    bool can_update_;
    bool is_pressed_;
    
public:
    InputAction();
    explicit InputAction(const EInputActionType type, const ECursorDataMode cursorMode = ECursorDataMode::Position, const float clampMax = 100.0f, const bool canUpdate = false, const float deadZone = 0.0f);
    ~InputAction();
    
    // Input events
    void Pressed();
    void Released();
    void Update(const float deltaTime) const; // Updates only happen while pressed for buttons, or in a frame where data changes for non buttons
    
    // Function binding
    void BindPressed(const std::function<void(const glm::vec2)>& lambda);
    void BindReleased(const std::function<void()>& lambda);
    void BindUpdated(const std::function<void(const glm::vec2, const float deltaTime)>& lambda);
    void Unbind();
    
    // Setters
    inline void                 SetClampMax(const float value) { clamp_max_ = value; }
    inline void                 SetDeadZone(const float value) { dead_zone_ = value; }
    inline void                 SetType(const EInputActionType type) { type_ = type; }
    inline void                 SetCursorDataMode(const ECursorDataMode mode) { cursor_data_mode_ = mode; }
    inline void                 SetCanUpdate(const bool flag) { can_update_ = flag; }
    
    // Getters
    inline float                GetClampMax() const { return clamp_max_; }
    inline float                GetDeadZone() const { return dead_zone_; }
    inline EInputActionType     GetType() const { return type_;}
    inline ECursorDataMode      GetCursorDataMode() const { return cursor_data_mode_; }
    inline bool                 CanUpdate() const { return can_update_; }
    inline bool                 IsPressed() const {return is_pressed_; }
};
