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
	std::map<unsigned int, std::vector<Widget*>> widgets_; // naturally maps are sorted by key on ascending order
	// Mouse
	InputAction cursor_input_action_;
	InputAction left_click_input_action_;
	InputAction right_click_input_action_;
	// Keyboard
	InputAction keyboard_buttons_input_actions_[6];
	// Gamepad
	InputAction gamepad_buttons_input_actions_[6];
	InputAction gamepad_axis_input_actions_[2];
	
	Widget* active_widget_;
	
	float axis_timer_;
	float axis_current_time_;
	
	bool active_;
	bool visible_;

public:
	
	Panel();
	virtual ~Panel();
	
	void AddWidget(const unsigned int layer, Widget& widget);
	void AddWidget(Widget& widget);
	
	inline void SetAxisTimer(const float second) { axis_timer_ = second; } // how much time has to pass before we process another axis input event from the keyboard
	inline void SetActive(const bool active) { active_ = active; }
	inline void SetVisible(const bool visible) { visible_ = visible; }
	inline void SelectWidget(Widget& widget) { active_widget_ = &widget; }
	
	inline bool IsActive() const { return active_; }
	inline bool IsVisible() const { return visible_; }
	
protected:
	void Start() const;
	void Update(const float deltaTime);
	void End() const;
	
private:
	// Mouse 
	void OnRightClick(const glm::vec2 data) const;
	void OnLeftClick(const glm::vec2 data) const;
	void OnRightClickReleased() const;
	void OnLeftClickReleased() const;
	void OnMouseMove(const glm::vec2 data, const float deltaTime) const;
	
	// Buttons
	void OnLeftPressed(const glm::vec2 data);
	void OnRightPressed(const glm::vec2 data);
	void OnUpPressed(const glm::vec2 data);
	void OnDownPressed(const glm::vec2 data);
	
	void OnSelectPressed(const glm::vec2 data) const;
	void OnReturnPressed(const glm::vec2 data) const;
	
	// Gamepad axes
	void OnAxisUpdated(const glm::vec2 data, const float deltaTime);
	
	void SearchForFocusTarget();
};
#endif