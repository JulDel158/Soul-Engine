#include "UI/Panel.hpp"

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include "EngineDataStructures.hpp"
#include "Input/InputManager.hpp"
#include "Physics/PhysicsDataStructures.hpp"
#include "Physics/PhysicsEngine.hpp"

namespace
{
	constexpr unsigned int LEFT_IA = 0;
	constexpr unsigned int RIGHT_IA = 1;
	constexpr unsigned int UP_IA = 2;
	constexpr unsigned int DOWN_IA = 3;
	
	constexpr unsigned int SELECT_IA = 4;
	constexpr unsigned int BACK_IA = 5;
	
	constexpr unsigned int AXIS_X_IA = 0;
	constexpr unsigned int AXIS_Y_IA = 1;
	
	constexpr float DEAD_ZONE = 0.75f;
	constexpr float DEFAULT_AXIS_TIMER = 0.5f;
}

Panel::Panel() :
axis_timer_(DEFAULT_AXIS_TIMER),
axis_current_time_(0.0f),
active_(true),
visible_(true)
{
	cursor_input_action_ = InputAction();
	auto& inputManager = InputManager::Instance();
	cursor_input_action_.SetType(EInputActionType::Mouse_Cursor);
	cursor_input_action_.SetCursorDataMode(ECursorDataMode::Position);
	cursor_input_action_.BindUpdated([this](const glm::vec2 data, const float deltaTime){ this->OnMouseMove(data, deltaTime); });
	inputManager.BindInputAction(&cursor_input_action_);
	
	left_click_input_action_.SetType(EInputActionType::Mouse_Button);
	left_click_input_action_.BindPressed([this](const glm::vec2 data){ this->OnLeftClick(data); });
	left_click_input_action_.BindReleased([this]() {this->OnLeftClickReleased();});
	inputManager.BindInputAction(&left_click_input_action_, GLFW_MOUSE_BUTTON_LEFT);
	
	right_click_input_action_.SetType(EInputActionType::Mouse_Button);
	right_click_input_action_.BindPressed([this](const glm::vec2 data){ this->OnRightClick(data); });
	inputManager.BindInputAction(&right_click_input_action_, GLFW_MOUSE_BUTTON_RIGHT);
	
	// initialize keyboard and controller button input actions
	for (int i = 0; i < 6; ++i)
	{
		keyboard_buttons_input_actions_[i].SetType(EInputActionType::Keyboard);
		gamepad_buttons_input_actions_[i].SetType(EInputActionType::Gamepad_Button);
	}
	
	
	auto Lleft_pressed = [this](const glm::vec2 data){this->OnLeftPressed(data);};
	auto Lright_pressed = [this](const glm::vec2 data){this->OnRightPressed(data);};
	auto Lup_pressed = [this](const glm::vec2 data){this->OnUpPressed(data);};
	auto Ldown_pressed = [this](const glm::vec2 data){this->OnDownPressed(data);};
	
	auto Lselect_pressed = [this](const glm::vec2 data){this->OnSelectPressed(data);};
	auto Lreturn_pressed = [this](const glm::vec2 data){this->OnReturnPressed(data);};
	
	keyboard_buttons_input_actions_[LEFT_IA].BindPressed(Lleft_pressed);
	keyboard_buttons_input_actions_[RIGHT_IA].BindPressed(Lright_pressed);
	keyboard_buttons_input_actions_[UP_IA].BindPressed(Lup_pressed);
	keyboard_buttons_input_actions_[DOWN_IA].BindPressed(Ldown_pressed);
	
	keyboard_buttons_input_actions_[SELECT_IA].BindPressed(Lselect_pressed);
	keyboard_buttons_input_actions_[BACK_IA].BindPressed(Lreturn_pressed);
	
	gamepad_buttons_input_actions_[LEFT_IA].BindPressed(Lleft_pressed);
	gamepad_buttons_input_actions_[RIGHT_IA].BindPressed(Lright_pressed);
	gamepad_buttons_input_actions_[UP_IA].BindPressed(Lup_pressed);
	gamepad_buttons_input_actions_[DOWN_IA].BindPressed(Ldown_pressed);
	
	gamepad_buttons_input_actions_[SELECT_IA].BindPressed(Lselect_pressed);
	gamepad_buttons_input_actions_[BACK_IA].BindPressed(Lreturn_pressed);
	
	for (auto& inputAction : gamepad_axis_input_actions_)
	{
		inputAction.SetType(EInputActionType::Gamepad_Axes);
		inputAction.SetDeadZone(DEAD_ZONE);
		inputAction.BindUpdated([this](const glm::vec2 data, const float deltaTime){this->OnAxisUpdated(data, deltaTime);});
	}
	
	inputManager.BindInputAction(&keyboard_buttons_input_actions_[LEFT_IA], glfwGetKeyScancode(GLFW_KEY_LEFT));
	inputManager.BindInputAction(&keyboard_buttons_input_actions_[RIGHT_IA], glfwGetKeyScancode(GLFW_KEY_RIGHT));
	inputManager.BindInputAction(&keyboard_buttons_input_actions_[UP_IA], glfwGetKeyScancode(GLFW_KEY_UP));
	inputManager.BindInputAction(&keyboard_buttons_input_actions_[DOWN_IA], glfwGetKeyScancode(GLFW_KEY_DOWN));
	inputManager.BindInputAction(&keyboard_buttons_input_actions_[SELECT_IA], glfwGetKeyScancode(GLFW_KEY_ENTER));
	inputManager.BindInputAction(&keyboard_buttons_input_actions_[BACK_IA], glfwGetKeyScancode(GLFW_KEY_ESCAPE));
	
	inputManager.BindInputAction(&gamepad_buttons_input_actions_[LEFT_IA], GLFW_GAMEPAD_BUTTON_DPAD_LEFT, GLFW_JOYSTICK_1);
	inputManager.BindInputAction(&gamepad_buttons_input_actions_[RIGHT_IA], GLFW_GAMEPAD_BUTTON_DPAD_RIGHT, GLFW_JOYSTICK_1);
	inputManager.BindInputAction(&gamepad_buttons_input_actions_[UP_IA], GLFW_GAMEPAD_BUTTON_DPAD_UP, GLFW_JOYSTICK_1);
	inputManager.BindInputAction(&gamepad_buttons_input_actions_[DOWN_IA], GLFW_GAMEPAD_BUTTON_DPAD_DOWN, GLFW_JOYSTICK_1);
	
	// We will bind these input actions twice, once for A/B and another time for Start/Back buttons
	inputManager.BindInputAction(&gamepad_buttons_input_actions_[SELECT_IA], GLFW_GAMEPAD_BUTTON_A, GLFW_JOYSTICK_1);
	inputManager.BindInputAction(&gamepad_buttons_input_actions_[BACK_IA], GLFW_GAMEPAD_BUTTON_B, GLFW_JOYSTICK_1);
	inputManager.BindInputAction(&gamepad_buttons_input_actions_[SELECT_IA], GLFW_GAMEPAD_BUTTON_START, GLFW_JOYSTICK_1);
	inputManager.BindInputAction(&gamepad_buttons_input_actions_[BACK_IA], GLFW_GAMEPAD_BUTTON_BACK, GLFW_JOYSTICK_1);
	
	inputManager.BindInputAction(&gamepad_axis_input_actions_[AXIS_X_IA], GLFW_GAMEPAD_AXIS_LEFT_X, GLFW_JOYSTICK_1);
	inputManager.BindInputAction(&gamepad_axis_input_actions_[AXIS_Y_IA], GLFW_GAMEPAD_AXIS_LEFT_Y, GLFW_JOYSTICK_1);
}

Panel::~Panel()
{
	widgets_.clear();
}

void Panel::AddWidget(const unsigned int layer, Widget& widget)
{
	widgets_[layer].push_back(&widget);
	widget.SetLayer(layer);
}

void Panel::AddWidget(Widget& widget)
{
	widgets_[widget.GetLayer()].push_back(&widget);
}

void Panel::Start() const
{
	for (const auto& layer : widgets_) //NOLINT
	{
		for (const auto& widget : layer.second)
		{
			widget->Start();
		}
	}
}

void Panel::Update(const float deltaTime)
{
	axis_current_time_ -= deltaTime;
	for (const auto& layer : widgets_) //NOLINT
	{
		for (const auto& widget : layer.second)
		{
			if (widget->IsActive() && widget->CanUpdate())
			{
				widget->Update(deltaTime);
			}
		}
	}
}

void Panel::End() const
{
	for (const auto& layer : widgets_) // NOLINT
	{
		for (const auto& widget : layer.second)
		{
			widget->End();
		}
	}
}

void Panel::OnRightClick(const glm::vec2 data) const
{
	if (!active_)
	{
		return;
	}
	for (const auto& layer : widgets_) // NOLINT
	{
		for (const auto& widget : layer.second)
		{
			if (!widget->IsActive() || !widget->IsListeningToInput())
			{
				continue;
			}
			if (PhysicsEngine::IsOverlapping(Quad(widget->position_, widget->size_, widget->scale_), data))
			{
				widget->OnClick(data, false);
			}
		}
	}
}

void Panel::OnLeftClick(const glm::vec2 data) const
{
	if (!active_)
	{
		return;
	}
	for (const auto& layer : widgets_) // NOLINT
	{
		for (const auto& widget : layer.second)
		{
			if (!widget->IsActive() || !widget->IsListeningToInput())
			{
				continue;
			}
			if (PhysicsEngine::IsOverlapping(Quad(widget->GetPosition(), widget->GetSize(), widget->GetScale()), data))
			{
				widget->OnClick(data,true);
			}
		}
	}
}

void Panel::OnRightClickReleased() const
{
	if (!active_)
	{
		return;
	}
	for (const auto& layer : widgets_) // NOLINT
	{
		for (const auto& widget : layer.second)
		{
			if (!widget->IsActive() || !widget->IsListeningToInput() || !widget->IsBeingClicked())
			{
				continue;
			}
			
			widget->OnClickRelease(false);
		}
	}
}

void Panel::OnLeftClickReleased() const
{
	if (!active_)
	{
		return;
	}
	for (const auto& layer : widgets_) // NOLINT
	{
		for (const auto& widget : layer.second)
		{
			if (!widget->IsActive() || !widget->IsListeningToInput() || !widget->IsBeingClicked())
			{
				continue;
			}
			
			widget->OnClickRelease(true);
		}
	}
}

void Panel::OnMouseMove(const glm::vec2 data, const float deltaTime) const
{
	if (!active_)
	{
		return;
	}
	for (const auto& layer : widgets_) // NOLINT
	{
		for (const auto& widget : layer.second)
		{
			if (!widget->IsActive() || !widget->IsListeningToInput())
			{
				continue;
			}
			if (PhysicsEngine::IsOverlapping(Quad(widget->position_, widget->size_, widget->scale_), data))
			{
				widget->OnMouseEnter();
			}
			else if (widget->IsOverlapping()) // if there is no collision this frame but overlapping flag is set, it means mouse left bounds
			{
				widget->OnMouseLeave();
			}
		}
	}
}

void Panel::OnLeftPressed(const glm::vec2 data)
{
	if (!active_)
	{
		return;
	}
	if (active_widget_ == nullptr)
	{
		SearchForFocusTarget();
		return;
	}
	Widget* target = active_widget_->GetNeighbor(EWidgetNeighbor::Left);
	if (target == nullptr) // Nothing happens
	{
		return;
	}
	
	active_widget_->OnUnfocused();
	target->OnFocused();
	active_widget_ = target;
}

void Panel::OnRightPressed(const glm::vec2 data)
{
	if (!active_)
	{
		return;
	}
	if (active_widget_ == nullptr)
	{
		SearchForFocusTarget();
		return;
	}
	Widget* target = active_widget_->GetNeighbor(EWidgetNeighbor::Right);
	if (target == nullptr) // Nothing happens
	{
		return;
	}
	
	active_widget_->OnUnfocused();
	target->OnFocused();
	active_widget_ = target;
}

void Panel::OnUpPressed(const glm::vec2 data)
{
	if (!active_)
	{
		return;
	}
	if (active_widget_ == nullptr)
	{
		SearchForFocusTarget();
		return;
	}
	Widget* target = active_widget_->GetNeighbor(EWidgetNeighbor::Up);
	if (target == nullptr) // Nothing happens
	{
		return;
	}
	
	active_widget_->OnUnfocused();
	target->OnFocused();
	active_widget_ = target;
}

void Panel::OnDownPressed(const glm::vec2 data)
{
	if (!active_)
	{
		return;
	}
	if (active_widget_ == nullptr)
	{
		SearchForFocusTarget();
		return;
	}
	Widget* target = active_widget_->GetNeighbor(EWidgetNeighbor::Down);
	if (target == nullptr) // Nothing happens
	{
		return;
	}
	
	active_widget_->OnUnfocused();
	target->OnFocused();
	active_widget_ = target;
}

void Panel::OnSelectPressed(const glm::vec2 data) const
{
	if (!active_)
	{
		return;
	}
	if (active_widget_ == nullptr)
	{
		return;
	}
	active_widget_->OnSelected();
}

void Panel::OnReturnPressed(const glm::vec2 data) const
{
	if (!active_)
	{
		return;
	}
	if (active_widget_ == nullptr)
	{
		return;
	}
	active_widget_->OnReturnPressed();
}

void Panel::OnAxisUpdated(const glm::vec2 data, const float deltaTime)
{
	if (!active_)
	{
		return;
	}
	if (active_widget_ == nullptr)
	{
		SearchForFocusTarget();
		return;
	}
	
	if (axis_current_time_ > 0.0f) // not enough time has passed, exit
	{
		return;
	}
	
	Widget* target = nullptr;
	// The Y axis will be checked first as it is often prioritized in menus
	if (data.y > 0.0f) // positive value goes down the screen
	{
		target = active_widget_->GetNeighbor(EWidgetNeighbor::Down);
	}
	else if (data.y < 0.0f) // going up
	{
		target = active_widget_->GetNeighbor(EWidgetNeighbor::Up);
	}
	else if (data.x > 0.0f) // going right
	{
		target = active_widget_->GetNeighbor(EWidgetNeighbor::Right);
	}
	else if (data.x < 0.0f) // going left
	{
		target = active_widget_->GetNeighbor(EWidgetNeighbor::Left);
	}
	if (target == nullptr) // No target was found
	{
		return;
	}
	
	active_widget_->OnUnfocused();
	target->OnFocused();
	active_widget_ = target;
	
	// reset the timer
	axis_current_time_ = axis_timer_;
}

void Panel::SearchForFocusTarget()
{
	for (std::pair<const unsigned int, std::vector<Widget*>>& layer : widgets_) // NOLINT
	{
		for (Widget*& widget : layer.second)
		{
			if (widget->IsActive() && widget->IsListeningToInput() && widget->CanBeFocused())
			{
				active_widget_ = widget;
				return;
			}
		}
	}
}
