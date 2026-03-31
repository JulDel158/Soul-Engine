#include "UI/Button.hpp"

#include "Components/SpriteComponent.hpp"
#include "Utils/ResourceManager.hpp"
#include "StringGlobals.hpp"

Button::Button() :
	left_click_function_(nullptr),
	left_click_released_function_(nullptr),
	right_click_function_(nullptr),
	right_click_released_function_(nullptr),
	selected_function_(nullptr),
	return_function_(nullptr),
	focused_function_(nullptr),
	unfocused_function_(nullptr),
	on_mouse_enter_function_(nullptr),
	on_mouse_leave_function_(nullptr), 
	sprite_component_(nullptr),
	label_offset_(glm::vec2(-25.f, -10.0f)),
	label_scale_(0.8f),
	label_color_(glm::vec4(1.0f)),
	label_base_color_(glm::vec4(1.0f)),
	label_on_click_color_(glm::vec4(1.0f,0.0f,0.0f,1.0f)),
	label_on_hover_color_(glm::vec4(0.0f,0.0f,1.0f,1.0f))

{
	can_listen_to_input_ = true;
	can_be_focused_ = true;
	scale_ = glm::vec2(1.0f);

	ResourceManager& resourceManager = ResourceManager::Instance();
	unsigned int temp;
	sprite_component_ = resourceManager.CreateComponent<SpriteComponent>(temp, nullptr);
	RegisterComponent(sprite_component_);
}

void Button::Update(const float deltaTime)
{
	Widget::Update(deltaTime);
	
	// TODO: Update text list
	text_list_.clear();
	text_list_.emplace_back(label_, label_offset_ + GetPosition(), label_scale_, label_color_, FONT_ARIAL.data());
	
	if (IsBeingClicked())
	{
		label_color_ = label_on_click_color_;
	}
	else if (IsHighlighted())
	{
		label_color_ = label_on_hover_color_;
	}
	else
	{
		label_color_ = label_base_color_;
	}
}

void Button::OnMouseEnter()
{
	Widget::OnMouseEnter();
	
	if (on_mouse_enter_function_ != nullptr)
	{
		on_mouse_enter_function_();
	}
}

void Button::OnMouseLeave()
{
	Widget::OnMouseLeave();
	
	if (on_mouse_leave_function_ != nullptr)
	{
		on_mouse_leave_function_();
	}
}

void Button::OnFocused()
{
	Widget::OnFocused();
	
	if (focused_function_ != nullptr)
	{
		focused_function_();
	}
}

void Button::OnUnfocused()
{
	Widget::OnUnfocused();
	
	if (unfocused_function_ != nullptr)
	{
		unfocused_function_();
	}
}

void Button::OnClick(const glm::vec2 mousePosition, const bool leftClick)
{
	Widget::OnClick(mousePosition, leftClick);
	
	if (leftClick)
	{
		if (left_click_function_ != nullptr)
		{
			left_click_function_();
		}
	}
	else
	{
		if (right_click_function_ != nullptr)
		{
			right_click_function_();
		}
	}
}

void Button::OnClickRelease(bool leftClick)
{
	Widget::OnClickRelease(leftClick);
}

void Button::OnSelected()
{
	Widget::OnSelected();
	
	if (selected_function_ != nullptr)
	{
		selected_function_();
	}
}

void Button::OnReturnPressed()
{
	Widget::OnReturnPressed();
	if (return_function_ != nullptr)
	{
		return_function_();
	}
}
