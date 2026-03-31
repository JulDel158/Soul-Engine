#ifndef BUTTON_HPP
#define BUTTON_HPP
#pragma once

#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float4.hpp"

#include "Widget.hpp"

#include <functional>
#include <string>
#include <utility>

class SpriteComponent;

class Button : public Widget
{
protected:
	std::function<void()> left_click_function_;
	std::function<void()> left_click_released_function_;
	std::function<void()> right_click_function_;
	std::function<void()> right_click_released_function_;
	std::function<void()> selected_function_;
	std::function<void()> return_function_;
	std::function<void()> focused_function_;
	std::function<void()> unfocused_function_;
	std::function<void()> on_mouse_enter_function_;
	std::function<void()> on_mouse_leave_function_;
	std::string label_;
	SpriteComponent* sprite_component_;
	glm::vec2 label_offset_;
	float label_scale_;
	glm::vec4 label_color_;
	glm::vec4 label_base_color_;
	glm::vec4 label_on_hover_color_;
	glm::vec4 label_on_click_color_;
	
public:
	Button();
	~Button() override = default;
	
	void Update(const float deltaTime) override;
	
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
	void OnClick(glm::vec2 mousePosition, bool leftClick) override;
	void OnClickRelease(bool leftClick) override;
	void OnSelected() override;
	void OnReturnPressed() override;
	
	void SetLabel(std::string text) { label_ = std::move(text); }
	void SetLabelOffset(const glm::vec2 offset) { label_offset_ = offset; }
	void SetLabelScale(const float size) { label_scale_ = size; }
	void SetLabelColor(const glm::vec4 color) { label_color_ = color; }
	void SetLabelOnClickColor(const glm::vec4 color) { label_on_click_color_ = color; }
	void SetLabelOnHoverColor(const glm::vec4 color) { label_on_hover_color_ = color; }
	void SetLabelBaseColor(const glm::vec4 color) { label_base_color_ = color; SetLabelColor(color); }
	
	SpriteComponent* GetSpriteComponent() const { return sprite_component_; }
	glm::vec2 GetLabelOffset() const { return label_offset_; }
	float GetLabelScale() const { return label_scale_; }
	glm::vec3 GetLabelColor() const { return label_color_; }
	
};


#endif