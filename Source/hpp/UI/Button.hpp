#ifndef BUTTON_HPP
#define BUTTON_HPP
#pragma once

#include "Widget.hpp"

#include <functional>
#include <string>
#include <utility>

class SpriteComponent;

class Button : public Widget
{
protected:
	std::function<void()> left_click_function_;
	std::function<void()> right_click_function_;
	std::function<void()> selected_function_;
	std::function<void()> return_function_;
	std::function<void()> focused_function_;
	std::function<void()> unfocused_function_;
	std::function<void()> on_mouse_enter_function_;
	std::function<void()> on_mouse_leave_function_;
	std::string label_;
	SpriteComponent* sprite_component_;
	
public:
	Button();
	~Button() override = default;
	
	void BindLeftClick(const std::function<void()>& function) { left_click_function_ = function; }
	void BindRightClick(const std::function<void()>& function) { right_click_function_ = function; }
	void BindSelected(const std::function<void()>& function) { selected_function_ = function; }
	void BindReturn(const std::function<void()>& function) { return_function_ = function; }
	void BindFocused(const std::function<void()>& function) { focused_function_ = function; }
	void BindUnfocused(const std::function<void()>& function) { unfocused_function_ = function; }
	void BindOnMouseEnter(const std::function<void()>& function) { on_mouse_enter_function_ = function; }
	void BindOnMouseLeaveFunction(const std::function<void()>& function) { on_mouse_leave_function_ = function; }
	
	void OnMouseEnter() override;
	void OnMouseLeave() override;
	void OnFocused() override;
	void OnUnfocused() override;
	void OnClick(bool leftClick) override;
	void OnSelected() override;
	void OnReturnPressed() override;
	
	void SetLabel(std::string text) { label_ = std::move(text); }
	
	SpriteComponent* GetSpriteComponent() const { return sprite_component_; }
	
};


#endif