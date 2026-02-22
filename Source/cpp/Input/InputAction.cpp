#include "Input/InputAction.hpp"

#include <iostream>
#include <utility>

InputAction::InputAction(): 
data_(glm::vec2(0.f)),
pressed_(nullptr),
released_(nullptr),
update_(nullptr),
clamp_max_(0.f),
type_(EInputActionType::None),
cursor_data_mode_(ECursorDataMode::Position),
is_pressed_(false),
can_update_(false)
{}

InputAction::InputAction(const EInputActionType type, const ECursorDataMode cursorMode, const float clampMax, bool canUpdate) :
data_(glm::vec2(0.f)),
pressed_(nullptr),
released_(nullptr),
update_(nullptr),
clamp_max_(clampMax),
type_(type),
cursor_data_mode_(cursorMode),
is_pressed_(false),
can_update_(canUpdate)
{
}

InputAction::~InputAction()
{
    Unbind();
}

void InputAction::Pressed()
{
    if (pressed_ != nullptr)
    {
        pressed_(data_);
    }
    is_pressed_ = true;
}

void InputAction::Released()
{
    data_ = glm::vec2(0.f);
    if (released_ != nullptr)
    {
        released_();
    }
    is_pressed_ = false;
}

void InputAction::Update(const float deltaTime) const
{
    if (is_pressed_ && can_update_ && update_ != nullptr)
    {
        update_(data_, deltaTime);
    }
}

void InputAction::BindPressed(const std::function<void(const glm::vec2&)>& lambda)
{
    pressed_ = lambda;
}

void InputAction::BindReleased(const std::function<void()>& lambda)
{
    released_ = lambda;
}

void InputAction::BindUpdated(const std::function<void(const glm::vec2&, const float& deltaTime)>& lambda)
{
    update_ = lambda;
}

void InputAction::Unbind()
{
    pressed_ = nullptr;
    released_ = nullptr;
    update_ = nullptr;
}

void InputAction::SetCanUpdate(const bool flag)
{
    can_update_ = flag;
}

void InputAction::SetClampMax(const float value)
{
    clamp_max_ = value;
}

void InputAction::SetType(const EInputActionType type)
{
    type_ = type;
}

void InputAction::SetCursorDataMode(const ECursorDataMode mode)
{
    cursor_data_mode_ = mode;
}

bool InputAction::IsPressed() const
{
    return is_pressed_;
}

bool InputAction::CanUpdate() const
{
    return can_update_ && update_ != nullptr;
}

float InputAction::GetClampMax() const
{
    return clamp_max_;
}

EInputActionType InputAction::GetType() const
{
    return type_;
}

ECursorDataMode InputAction::GetCursorDataMode() const
{
    return cursor_data_mode_;
}
