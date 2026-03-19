#ifndef CHARACTER_HPP
#define CHARACTER_HPP
#pragma once

#include "Game/GameObject.hpp"

class SpriteComponent;
class MovementComponent;

// Base class for characters
class Character : public GameObject
{
protected:
	SpriteComponent* sprite_component_;
	MovementComponent* movement_component_;
	
public:
	Character();
	~Character() override = default;
	
protected:
	void InitializeComponents();
};

#endif