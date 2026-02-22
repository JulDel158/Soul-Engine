#ifndef INPUT_ACTION_H
#define INPUT_ACTION_H

#include "EngineDataStructures.hpp"

#include <string>
#include <functional>

class InputAction
{
public:
    UInputData data_;
    
    InputAction();
    explicit InputAction(std::string name);
    ~InputAction();
    
    // TODO: Receive event
    
    void Pressed();
    void Released();
    void Update(const float deltaTime) const; // Updates only happen while pressed
    
    void BindPressed(const std::function<void(const UInputData&)>& lambda);
    void BindReleased(const std::function<void()>& lambda);
    void BindUpdated(const std::function<void(const UInputData&, const float& deltaTime)>& lambda);
    
    void Unbind();
    void SetCanUpdate(const bool shouldUpdate);
    bool CanUpdate() const;

private:
    
    std::function<void(const UInputData&)> pressed_;
    std::function<void()> released_;
    std::function<void(const UInputData&, const float& deltaTime)> update_;
    std::string name_;
    bool is_pressed_;
    bool should_update_;
};


#endif
