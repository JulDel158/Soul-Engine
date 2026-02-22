#include "Input/InputAction.hpp"

#include <iostream>
#include <utility>

InputAction::InputAction(): 
data_{},
name_("GENERIC INPUT ACTION"),
pressed_(nullptr),
released_(nullptr),
update_(nullptr),
is_pressed_(false),
should_update_(false)
{}

InputAction::InputAction(std::string name):
data_{},
name_(std::move(name)),
pressed_(nullptr),
released_(nullptr),
update_(nullptr),
is_pressed_(false),
should_update_(false)
{}

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
    data_.vec2 = {0.f,0.f};
    if (released_ != nullptr)
    {
        released_();
    }
    is_pressed_ = false;
}

void InputAction::Update(const float deltaTime) const
{
    if (is_pressed_ && should_update_ && update_ != nullptr)
    {
        update_(data_, deltaTime);
    }
}

void InputAction::BindPressed(const std::function<void(const UInputData&)>& lambda)
{
    pressed_ = lambda;
}

void InputAction::BindReleased(const std::function<void()>& lambda)
{
    released_ = lambda;
}

void InputAction::BindUpdated(const std::function<void(const UInputData&, const float& deltaTime)>& lambda)
{
    update_ = lambda;
}

void InputAction::Unbind()
{
    pressed_ = nullptr;
    released_ = nullptr;
    update_ = nullptr;
}

void InputAction::SetCanUpdate(const bool shouldUpdate)
{
    should_update_ = shouldUpdate;
}

bool InputAction::CanUpdate() const
{
    return should_update_ && update_ != nullptr;
}
