#ifndef ABILITY_HPP
#define ABILITY_HPP
#pragma once

#include "GameDataStructures.hpp"

#include <vector>

class CombatCharacter;

class Ability
{
protected:
	int cost_;
	CombatCharacter* owner_;
	ETargetingType targeting_type_;
	EActionRange range_;
	
public:
	Ability();
	virtual ~Ability();
	
	virtual void Execute(std::vector<CombatCharacter&>& enemies, std::vector<CombatCharacter&>& allies);
};

#endif