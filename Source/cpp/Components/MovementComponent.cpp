#include "Components/MovementComponent.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"

#include "EngineDataStructures.hpp"
#include "Game/GameObject.hpp"

MovementComponent::MovementComponent() : 
movement_speed_(0.0f), 
walking_speed_(1.0f), 
running_speed_(1.0f), 
swimming_speed_(1.0f),
movement_as_scale_(false),
movement_mode_(EMovementMode::None)
{
	component_type_ = EComponentClassType::MovementComponent;
}

bool MovementComponent::IsMovingInDirection(EMovementDirection direction) const
{
	bool result = false;
	
	switch (direction)
	{
		// up = going lower on screen, down = going higher on screen
		case EMovementDirection::Up:
		result = direction_.y < 0.0f;
		break;
		case EMovementDirection::Down:
		result = direction_.y > 0.0f;
		break;
		case EMovementDirection::Left:
		result = direction_.x < 0.0f;
		break;
		case EMovementDirection::Right:
		result = direction_.x > 0.0f;
		break;
	case EMovementDirection::None:
		break;
	}
	
	return result;
}

bool MovementComponent::IsMoving() const
{
	return glm::length2(direction_) > 0.0f;
}

void MovementComponent::Start()
{
	BaseComponent::Start();
}

void MovementComponent::Update(const float deltaTime)
{
	BaseComponent::Update(deltaTime);
	if (owner_ == nullptr)
	{
		return;
	}
	
	auto position = owner_->GetPosition();
	float speed = movement_speed_;
	
	switch (movement_mode_)
	{
	case EMovementMode::Walking:
		speed = (movement_as_scale_) ? speed * walking_speed_: walking_speed_;
		break;
	case EMovementMode::Running:
		speed = (movement_as_scale_) ? speed * running_speed_: running_speed_;
		break;
	case EMovementMode::Swimming:
		speed = (movement_as_scale_) ? speed * swimming_speed_: swimming_speed_;
		break;
	case EMovementMode::None:
		break;
	}
	
	position += direction_ * speed * deltaTime;
	owner_->SetPosition(position);
}

void MovementComponent::End()
{
	BaseComponent::End();
}
