#ifndef INPUT_ACTION_H
#define INPUT_ACTION_H

#include "EngineDataStructures.hpp"

#include <functional>

#include "glm/vec2.hpp"

class InputAction
{
public:
    glm::vec2 data_;
    
    InputAction();
    explicit InputAction(const EInputActionType type = EInputActionType::None, const ECursorDataMode cursorMode = ECursorDataMode::Position, const float clampMax = 100.0f, bool canUpdate = false, float deadZone = 0.0f);
    ~InputAction();
    
    void Pressed();
    void Released();
    void Update(const float deltaTime) const; // Updates only happen while pressed
    
    void BindPressed(const std::function<void(const glm::vec2&)>& lambda);
    void BindReleased(const std::function<void()>& lambda);
    void BindUpdated(const std::function<void(const glm::vec2&, const float& deltaTime)>& lambda);
    void Unbind();
    
    void SetClampMax(const float value);
    void SetDeadZone(const float value);
    void SetType(const EInputActionType type);
    void SetCursorDataMode(const ECursorDataMode mode);
    void SetCanUpdate(const bool flag);
    
    float               GetClampMax() const;
    float               GetDeadZone() const;
    EInputActionType    GetType() const;
    ECursorDataMode     GetCursorDataMode() const;
    bool                IsPressed() const;
    bool                CanUpdate() const;

private:
    
    std::function<void(const glm::vec2&)> function_pressed_;
    std::function<void()> function_released_;
    std::function<void(const glm::vec2&, const float& deltaTime)> function_update_;
    float clamp_max_;
    float dead_zone_;
    EInputActionType type_;
    ECursorDataMode cursor_data_mode_;
    bool is_pressed_;
    bool can_update_;
};


#endif
