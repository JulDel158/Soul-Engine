#ifndef MOVEMENT_COMPONENT_HPP
#define MOVEMENT_COMPONENT_HPP
#pragma once

#include "glm/ext/vector_float2.hpp"

#include "BaseComponent.hpp"
#include "GameDataStructures.hpp"


class MovementComponent : public BaseComponent
{
	glm::vec2 direction_;
	float movement_speed_;
	float walking_speed_;
	float running_speed_;
	float swimming_speed_;
	bool movement_as_scale_;
	EMovementMode movement_mode_;
	
public:
	MovementComponent();
	~MovementComponent() override = default;
	
	// Setters
	/* If true: walking, running and swimming values will be used as a scaled multiplied to movement_speed. 
	*  If false: movement speed is ignored and instead walking, running, swimming speed is used based on mode
	*/
	inline void UseSpeedAsScales(const bool value)	{ movement_as_scale_ = value; }
	inline void SetMovementSpeed(const float speed) { movement_speed_ = speed; }
	inline void SetWalkingSpeed(const float value)	{ walking_speed_ = value; }
	inline void SetRunningSpeed(const float value)	{ running_speed_ = value; }
	inline void SetSwimmingSpeed(const float value) { swimming_speed_ = value; }
	inline void SetMovementDirection(glm::vec2 direction) { direction_ = direction; }
	inline void SetMovementMode(const EMovementMode mode) { movement_mode_ = mode; }
	
	// Getters
	bool IsMovingInDirection(EMovementDirection direction) const;
	bool IsMoving() const;
	inline bool IsWalking() const	{ return movement_mode_ == EMovementMode::Walking; }
	inline bool IsRunning() const	{ return movement_mode_ == EMovementMode::Running; }
	inline bool IsSwimming() const	{ return movement_mode_ == EMovementMode::Swimming;}
	inline glm::vec2 GetDirection() const { return direction_; }
	
	
protected:
	void Start() override;
	void Update(const float deltaTime) override;
	void End() override;
};

#endif
