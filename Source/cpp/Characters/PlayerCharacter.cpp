#include "Characters/PlayerCharacter.hpp"

#include "glad/gl.h" // NOLINT
#include "GLFW/glfw3.h"

#include "Rendering/SpriteAnimation.hpp"
#include "EngineDataStructures.hpp"
#include "GameDataStructures.hpp"
#include "Components/SpriteComponent.hpp"
#include "Components/MovementComponent.hpp"
#include "Utils/ResourceManager.hpp"
#include "Input/InputManager.hpp"

#include <vector>

namespace
{
	constexpr int NO_OWNER = 0;
	constexpr int LEFT_KEY = -1;
	constexpr int RIGHT_KEY = 1;
	constexpr int UP_KEY = -1;
	constexpr int DOWN_KEY = 1;
}

PlayerCharacter::PlayerCharacter()
{
	gamepad_mode_ = false;
	toggle_sprint_ = true;
	y_owner_ = x_owner_ = NO_OWNER;
	
	auto& resourceManager = ResourceManager::Instance();
	// TODO: Set animations and sprites for player
	sprite_component_->SetDefaultTexture(resourceManager.GetTexture2D(ESpriteKey::Player_Idle_1));
	
	std::vector<ESpriteKey> spriteKeys;
	spriteKeys.reserve(3);
	spriteKeys.push_back(ESpriteKey::Player_Idle_1);
	spriteKeys.push_back(ESpriteKey::Player_Idle_2);
	spriteKeys.push_back(ESpriteKey::Player_Idle_3);
	sprite_component_->AddAnimation(static_cast<int>(EPlayerAnimationState::Idle), SpriteAnimation(spriteKeys, 30));
	
	spriteKeys.clear();
	spriteKeys.push_back(ESpriteKey::Player_Left_1);
	spriteKeys.push_back(ESpriteKey::Player_Left_2);
	spriteKeys.push_back(ESpriteKey::Player_Left_3);
	sprite_component_->AddAnimation(static_cast<int>(EPlayerAnimationState::Walking_Left), SpriteAnimation(spriteKeys, 30));
	
	spriteKeys.clear();
	spriteKeys.push_back(ESpriteKey::Player_Right_1);
	spriteKeys.push_back(ESpriteKey::Player_Right_2);
	spriteKeys.push_back(ESpriteKey::Player_Right_3);
	sprite_component_->AddAnimation(static_cast<int>(EPlayerAnimationState::Walking_Right), SpriteAnimation(spriteKeys, 30));
	
	spriteKeys.clear();
	spriteKeys.push_back(ESpriteKey::Player_Up_1);
	spriteKeys.push_back(ESpriteKey::Player_Up_2);
	spriteKeys.push_back(ESpriteKey::Player_Up_3);
	sprite_component_->AddAnimation(static_cast<int>(EPlayerAnimationState::Walking_Up), SpriteAnimation(spriteKeys, 30));
	
	spriteKeys.clear();
	spriteKeys.push_back(ESpriteKey::Player_Down_1);
	spriteKeys.push_back(ESpriteKey::Player_Down_2);
	spriteKeys.push_back(ESpriteKey::Player_Down_3);
	sprite_component_->AddAnimation(static_cast<int>(EPlayerAnimationState::Walking_Down), SpriteAnimation(spriteKeys, 30));
	
	InitializeInputActions();
}

void PlayerCharacter::OnMovementInputUpdate(glm::vec2 data, const bool keepCurrentX, const bool keepCurrentY) const
{
	// This is to ensure gamepad values are always either -1, 0, or 1
	if (data.x > 0.0f)
	{
		data.x = 1.0f;
	}
	else if (data.x < 0.0f)
	{
		data.x = -1.0f;
	}
	
	if (data.y > 0.0f)
	{
		data.y = 1.0f;
	}
	else if (data.y < 0.0f)
	{
		data.y = -1.0f;
	}
	
	glm::vec2 result = movement_component_->GetDirection();
	if (!keepCurrentX)
	{
		result.x = data.x;
	}
	
	if (!keepCurrentY)
	{
		result.y = data.y;
	}
	
	movement_component_->SetMovementDirection(result);
}

void PlayerCharacter::SprintPressed() const
{
	movement_component_->SetMovementMode(EMovementMode::Running);
}

void PlayerCharacter::SprintReleased() const
{
	movement_component_->SetMovementMode(EMovementMode::Walking);
}

void PlayerCharacter::InitializeInputActions()
{
	auto& inputManager = InputManager::Instance();
	
	gamepad_movement_ia_.SetType(EInputActionType::Gamepad_Axes);
	gamepad_sprint_ia_.SetType(EInputActionType::Gamepad_Button);
	keyboard_up_ia_.SetType(EInputActionType::Keyboard);
	keyboard_down_ia_.SetType(EInputActionType::Keyboard);
	keyboard_left_ia_.SetType(EInputActionType::Keyboard);
	keyboard_right_ia_.SetType(EInputActionType::Keyboard);
	keyboard_sprint_ia_.SetType(EInputActionType::Keyboard);
	
	gamepad_movement_ia_.BindUpdated([this](const glm::vec2 data, const float deltaTime)
	{
		if (!gamepad_mode_)
		{
			return;
		}
		this->OnMovementInputUpdate(data, false, false);
		x_owner_ = (data.x > 0.0f) ? RIGHT_KEY : (data.x < 0.0f) ? LEFT_KEY : NO_OWNER;
		y_owner_ = (data.y > 0.0f) ? DOWN_KEY : (data.y < 0.0f) ? UP_KEY : NO_OWNER;
		this->UpdateAnimationState();
	});
	
	gamepad_sprint_ia_.BindPressed([this](const glm::vec2 data)
	{
		if (!gamepad_mode_)
		{
			return;
		}
		
		this->SprintPressed();
	});
	gamepad_sprint_ia_.BindReleased([this]()
	{
		if (!gamepad_mode_)
		{
			return;
		}
		
		this->SprintReleased();
	});
	
	
	keyboard_up_ia_.BindPressed([this](const glm::vec2 data)
	{
		if (gamepad_mode_)
		{
			return;
		}
		if (y_owner_ == NO_OWNER)
		{
			this->OnMovementInputUpdate(glm::vec2(0.0f, -1.0f), true, false);
			y_owner_ = UP_KEY;
			this->UpdateAnimationState();
		}
	});
	keyboard_up_ia_.BindReleased([this]()
	{
		if (gamepad_mode_)
		{
			return;
		}
		if (y_owner_ == UP_KEY)
		{
			this->OnMovementInputUpdate(glm::vec2(0.0f, 0.0f), true, false);
			y_owner_ = NO_OWNER;
		}
		
		// now we check if down is pressed, if so give them ownership
		if (keyboard_down_ia_.IsPressed())
		{
			this->OnMovementInputUpdate(glm::vec2(0.0f, 1.0f), true, false);
			y_owner_ = DOWN_KEY;
		}
		
		this->UpdateAnimationState();
	});
	
	keyboard_down_ia_.BindPressed([this](const glm::vec2 data)
	{
		if (gamepad_mode_)
		{
			return;
		}
		if (y_owner_ == NO_OWNER)
		{
			this->OnMovementInputUpdate(glm::vec2(0.0f, 1.0f), true, false);
			y_owner_ = DOWN_KEY;
			this->UpdateAnimationState();
		}
	});
	keyboard_down_ia_.BindReleased([this]()
	{
		if (gamepad_mode_)
		{
			return;
		}
		if (y_owner_ == DOWN_KEY)
		{
			this->OnMovementInputUpdate(glm::vec2(0.0f, 0.0f), true, false);
			y_owner_ = NO_OWNER;
		}
		// now we check if up is pressed, if so give them ownership
		if (keyboard_up_ia_.IsPressed())
		{
			this->OnMovementInputUpdate(glm::vec2(0.0f, -1.0f), true, false);
			y_owner_ = UP_KEY;
		}
		
		this->UpdateAnimationState();
	});
	
	keyboard_left_ia_.BindPressed([this](const glm::vec2 data)
	{
		if (gamepad_mode_)
		{
			return;
		}
		if (x_owner_ == NO_OWNER)
		{
			this->OnMovementInputUpdate(glm::vec2(-1.0f, 0.0f), false, true);
			x_owner_ = LEFT_KEY;
			this->UpdateAnimationState();
		}
	});
	keyboard_left_ia_.BindReleased([this]()
	{
		if (gamepad_mode_)
		{
			return;
		}
		if (x_owner_ == LEFT_KEY)
		{
			this->OnMovementInputUpdate(glm::vec2(0.0f, 0.0f), false, true);
			x_owner_ = NO_OWNER;
		}
		
		// now we check if right is pressed, if so give them ownership
		if (keyboard_right_ia_.IsPressed())
		{
			this->OnMovementInputUpdate(glm::vec2(1.0f, 0.0f), false, true);
			x_owner_ = RIGHT_KEY;
		}
		this->UpdateAnimationState();
	});
	
	keyboard_right_ia_.BindPressed([this](const glm::vec2 data)
	{
		if (gamepad_mode_)
		{
			return;
		}
		if (x_owner_ == NO_OWNER)
		{
			this->OnMovementInputUpdate(glm::vec2(1.0f, 0.0f), false, true);
			x_owner_ = RIGHT_KEY;
			this->UpdateAnimationState();
		}
	});
	keyboard_right_ia_.BindReleased([this]()
	{
		if (gamepad_mode_)
		{
			return;
		}
		if (x_owner_ == RIGHT_KEY)
		{
			this->OnMovementInputUpdate(glm::vec2(0.0f, 0.0f), false, true);
			x_owner_ = NO_OWNER;
		}
		// now we check if left is pressed, if so give them ownership
		if (keyboard_left_ia_.IsPressed())
		{
			this->OnMovementInputUpdate(glm::vec2(-1.0f, 0.0f), false, true);
			x_owner_ = LEFT_KEY;
		}
		this->UpdateAnimationState();
	});
	
	keyboard_sprint_ia_.BindPressed([this](const glm::vec2 data)
	{
		if (gamepad_mode_)
		{
			return;
		}
		
		this->SprintPressed();
	});
	keyboard_sprint_ia_.BindReleased([this]()
	{
		if (gamepad_mode_)
		{
			return;
		}
		
		this->SprintReleased();
	});
	
	inputManager.BindInputAction(&gamepad_movement_ia_, GLFW_GAMEPAD_AXIS_LEFT_X, GLFW_JOYSTICK_1);
	inputManager.BindInputAction(&gamepad_movement_ia_, GLFW_GAMEPAD_AXIS_LEFT_Y, GLFW_JOYSTICK_1);
	inputManager.BindInputAction(&gamepad_sprint_ia_, GLFW_GAMEPAD_BUTTON_X, GLFW_JOYSTICK_1);
	
	inputManager.BindInputAction(&keyboard_up_ia_, glfwGetKeyScancode(GLFW_KEY_W));
	inputManager.BindInputAction(&keyboard_down_ia_, glfwGetKeyScancode(GLFW_KEY_S));
	inputManager.BindInputAction(&keyboard_left_ia_, glfwGetKeyScancode(GLFW_KEY_A));
	inputManager.BindInputAction(&keyboard_right_ia_, glfwGetKeyScancode(GLFW_KEY_D));
	inputManager.BindInputAction(&keyboard_sprint_ia_, glfwGetKeyScancode(GLFW_KEY_LEFT_SHIFT));
}

void PlayerCharacter::UpdateAnimationState() const
{
	// Left and right have priority over up and down
	if (x_owner_ == LEFT_KEY)
	{
		sprite_component_->PlayAnimation(static_cast<int>(EPlayerAnimationState::Walking_Left));
	}
	else if (x_owner_ == RIGHT_KEY)
	{
		sprite_component_->PlayAnimation(static_cast<int>(EPlayerAnimationState::Walking_Right));
	}
	else if (y_owner_ == UP_KEY)
	{
		sprite_component_->PlayAnimation(static_cast<int>(EPlayerAnimationState::Walking_Up));
	}
	else if (y_owner_ == DOWN_KEY)
	{
		sprite_component_->PlayAnimation(static_cast<int>(EPlayerAnimationState::Walking_Down));
	}
	else
	{
		sprite_component_->PlayAnimation(static_cast<int>(EPlayerAnimationState::Idle));
	}
	
}

void PlayerCharacter::SwitchInputMode()
{
	gamepad_mode_ = !gamepad_mode_;
	y_owner_ = x_owner_ = NO_OWNER;
	SprintPressed();
	OnMovementInputUpdate(glm::vec2(0.0f, 0.0f), false, false);
	UpdateAnimationState();
}
