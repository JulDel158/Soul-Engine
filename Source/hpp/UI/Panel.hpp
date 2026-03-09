#ifndef PANEL_HPP
#define PANEL_HPP
#pragma once

#include "UI/Widget.hpp"
#include "Input/InputAction.hpp"

#include <map>
#include <vector>

class Panel
{
protected:
	friend class Game;
	std::map<unsigned int, std::vector<Widget*>> widgets_; // naturally maps are sorted by key on descending order
	// Mouse
	InputAction cursor_input_action_;
	InputAction left_click_input_action_;
	InputAction right_click_input_action_;
	// Keyboard
	InputAction keyboard_buttons_input_actions_[6];
	// Gamepad
	InputAction gamepad_buttons_input_actions_[6];
	InputAction gamepad_axis_input_actions_[2];
	
	//TODO: Add logic for widget navigation within a panel!
	
	bool active_;
	bool visible_;

public:
	
	Panel();
	virtual ~Panel();
	
	void AddWidget(const unsigned int layer, Widget& widget);
	void AddWidget(Widget& widget);
	
	inline void SetActive(const bool active) { active_ = active; }
	inline void SetVisible(const bool visible) { visible_ = visible; }
	
	inline bool IsActive() const { return active_; }
	inline bool IsVisible() const { return visible_; }
	
protected:
	void Start() const;
	void Update(const float deltaTime) const;
	void End() const;
	
private:
	// Mouse 
	void OnRightClick(const glm::vec2 data) const;
	void OnLeftClick(const glm::vec2 data) const;
	void OnMouseMove(const glm::vec2 data, const float deltaTime) const;
	
	// Buttons
	void OnLeftPressed(const glm::vec2 data) const;
	void OnRightPressed(const glm::vec2 data) const;
	void OnUpPressed(const glm::vec2 data) const;
	void OnDownPressed(const glm::vec2 data) const;
	
	void OnSelectPressed(const glm::vec2 data) const;
	void OnReturnPressed(const glm::vec2 data) const;
	
	// Gamepad axes
	void OnXAxisUpdated(const glm::vec2 data, const float deltaTime) const;
	void OnYAxisUpdated(const glm::vec2 data, const float deltaTime) const;
};
#endif