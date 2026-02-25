#include "Input/InputAction.hpp"

namespace {
    constexpr auto ZERO_VECTOR = glm::vec2(0.0f);
}

InputAction::InputAction(): 
data_(ZERO_VECTOR),
function_pressed_(nullptr),
function_released_(nullptr),
function_update_(nullptr),
clamp_max_(0.f),
dead_zone_(0.f),
type_(EInputActionType::None),
cursor_data_mode_(ECursorDataMode::Position),
can_update_(false),
is_pressed_(false)
{}

InputAction::InputAction(const EInputActionType type, const ECursorDataMode cursorMode, const float clampMax, const bool canUpdate, const float deadZone) :
data_(ZERO_VECTOR),
function_pressed_(nullptr),
function_released_(nullptr),
function_update_(nullptr),
clamp_max_(clampMax),
dead_zone_(deadZone),
type_(type),
cursor_data_mode_(cursorMode),
can_update_(canUpdate),
is_pressed_(false)
{}

InputAction::~InputAction()
{
    Unbind();
}

void InputAction::Pressed()
{
    if (function_pressed_ != nullptr)
    {
        function_pressed_(data_);
    }
    is_pressed_ = true;
}

void InputAction::Released()
{
    data_ = ZERO_VECTOR;
    if (function_released_ != nullptr)
    {
        function_released_();
    }
    is_pressed_ = false;
}

void InputAction::Update(const float deltaTime) const
{
    switch (type_)
    {
        // for button types we want to ensure they are being pressed, no point on updating buttons when they aren't being used
    case EInputActionType::Keyboard:
    case EInputActionType::Mouse_Button:
    case EInputActionType::Gamepad_Button:
        {
            if ((is_pressed_) && can_update_ && function_update_ != nullptr)
            {
                function_update_(data_, deltaTime);
            }
            break;
        }
    case EInputActionType::Mouse_Wheel:
    case EInputActionType::Mouse_Cursor:
    case EInputActionType::Gamepad_Axes:
        {
            if (can_update_ && function_update_ != nullptr)
            {
                function_update_(data_, deltaTime);
            }
            break;
        }
    case EInputActionType::None:
        {
            break;
        }
    }
}

void InputAction::BindPressed(const std::function<void(const glm::vec2)>& lambda)
{
    function_pressed_ = lambda;
}

void InputAction::BindReleased(const std::function<void()>& lambda)
{
    function_released_ = lambda;
}

void InputAction::BindUpdated(const std::function<void(const glm::vec2, const float deltaTime)>& lambda)
{
    function_update_ = lambda;
}

void InputAction::Unbind()
{
    function_pressed_ = nullptr;
    function_released_ = nullptr;
    function_update_ = nullptr;
}
