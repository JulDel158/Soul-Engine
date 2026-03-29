#include "UI/Button.hpp"

#include "Components/SpriteComponent.hpp"
#include "Utils/ResourceManager.hpp"

Button::Button() :
left_click_function_(nullptr),
right_click_function_(nullptr),
selected_function_(nullptr),
return_function_(nullptr),
focused_function_(nullptr),
unfocused_function_(nullptr),
on_mouse_enter_function_(nullptr),
on_mouse_leave_function_(nullptr)
{
	can_listen_to_input_ = true;
	can_be_focused_ = true;
	
	ResourceManager& resourceManager = ResourceManager::Instance();
	unsigned int temp;
	resourceManager.CreateComponent<SpriteComponent>(temp, nullptr);
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

void Button::OnClick(const bool leftClick)
{
	Widget::OnClick(leftClick);
	
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
