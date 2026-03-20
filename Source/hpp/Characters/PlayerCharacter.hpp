#ifndef PLAYER_CHARACTER_HPP
#define PLAYER_CHARACTER_HPP
#pragma once

#include "glm/ext/vector_float2.hpp"

#include "Character.hpp"
#include "Input/InputAction.hpp"

class PlayerCharacter : public Character
{
	InputAction gamepad_movement_ia_;
	InputAction gamepad_sprint_ia_;
	InputAction keyboard_up_ia_;
	InputAction keyboard_down_ia_;
	InputAction keyboard_left_ia_;
	InputAction keyboard_right_ia_;
	InputAction keyboard_sprint_ia_;
	int y_owner_;
	int x_owner_;
	bool gamepad_mode_;
	bool toggle_sprint_;
	bool is_swimming_; // TODO: handle swimming logic
	
public:
	PlayerCharacter();
	~PlayerCharacter() override = default;
	
	inline void SetSprintAsToggle(const bool toggle) { toggle_sprint_ = toggle; }
	inline bool IsSprintToggle() const { return toggle_sprint_; }
	
private:
	void OnMovementInputUpdate(glm::vec2 data, bool keepCurrentX, bool keepCurrentY) const;
	void SprintPressed() const;
	void SprintReleased() const;
	void InitializeInputActions();
	void UpdateAnimationState() const;
	void SwitchInputMode();
};

#endif
