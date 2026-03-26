#ifndef COMBAT_CHARACTER_HPP
#define COMBAT_CHARACTER_HPP
#pragma once

#include "Character.hpp"
#include "GameDataStructures.hpp"

class HealthComponent;

class CombatCharacter : public Character
{
protected:
	HealthComponent* health_component_;
	
public:
	CombatCharacter();
	~CombatCharacter() override = default;
	
	inline HealthComponent* GetHealthComponent() const { return health_component_; }
	
private:
	void InitComponents();
	// TODO: Add combat character base attributes
};

#endif
