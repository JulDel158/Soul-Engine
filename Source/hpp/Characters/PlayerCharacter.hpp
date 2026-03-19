#ifndef PLAYER_CHARACTER_HPP
#define PLAYER_CHARACTER_HPP
#pragma once

#include "glm/ext/vector_float2.hpp"

#include "Character.hpp"
#include "Input/InputAction.hpp"

class PlayerCharacter : public Character
{
	InputAction gamepad_movement_ia_;
	InputAction keyboard_up_ia_;
	InputAction keyboard_down_ia_;
	InputAction keyboard_left_ia_;
	InputAction keyboard_right_ia_;
	bool gamepad_mode_;
	int x_owner_;
	int y_owner_;
public:
	PlayerCharacter();
	~PlayerCharacter() override = default;
	
private:
	void OnMovementInputUpdate(glm::vec2 data, bool keepCurrentX, bool keepCurrentY) const;
	void InitializeInputActions();
};

#endif
