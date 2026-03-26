#ifndef CHARACTER_HPP
#define CHARACTER_HPP
#pragma once

#include "Game/GameObject.hpp"

class SpriteComponent;
class MovementComponent;
class BoxCollisionComponent;

// Base class for characters
class Character : public GameObject
{
protected:
	SpriteComponent* sprite_component_;
	MovementComponent* movement_component_;
	BoxCollisionComponent* box_collision_component_;
	
public:
	Character();
	~Character() override = default;
	
private:
	void InitializeComponents();
};

#endif