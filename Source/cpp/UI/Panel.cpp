#include "UI/Panel.hpp"

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include "EngineDataStructures.hpp"
#include "Input/InputManager.hpp"

Panel::Panel() :
active_(true),
visible_(true)
{
	cursor_input_action_ = InputAction();
	auto& inputManager = InputManager::Instance();
	cursor_input_action_.SetType(EInputActionType::Mouse_Cursor);
	cursor_input_action_.SetCursorDataMode(ECursorDataMode::Position);
	cursor_input_action_.BindUpdated([this](const glm::vec2& data, const float deltaTime){ this->OnMouseMove(data, deltaTime); });
	inputManager.BindInputAction(&cursor_input_action_);
	
	left_click_input_action_.SetType(EInputActionType::Mouse_Button);
	left_click_input_action_.BindPressed([this](const glm::vec2& data){ this->OnLeftClick(data); });
	inputManager.BindInputAction(&left_click_input_action_, GLFW_MOUSE_BUTTON_LEFT);
	
	right_click_input_action_.SetType(EInputActionType::Mouse_Button);
	right_click_input_action_.BindPressed([this](const glm::vec2& data){ this->OnRightClick(data); });
	inputManager.BindInputAction(&right_click_input_action_, GLFW_MOUSE_BUTTON_RIGHT);
}

Panel::~Panel()
{
	widgets_.clear();
}

void Panel::AddWidget(const unsigned int layer, Widget& widget)
{
	widgets_[layer].push_back(&widget);
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

void Panel::Update(const float deltaTime) const
{
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

// TODO: Check collision and process input events
void Panel::OnRightClick(const glm::vec2 data)
{
}

void Panel::OnLeftClick(const glm::vec2 data)
{
}

void Panel::OnMouseMove(const glm::vec2 data, const float deltaTime)
{
}
