#ifndef COMBAT_ACTION_HPP
#define COMBAT_ACTION_HPP
#pragma once

#include "GameDataStructures.hpp"
#include "CombatModifier.hpp"

#include <vector>


class CombatAction
{
public:
	ETargetingType targeting_type_;
	
	// The modifiers that get applied to the targets
	std::vector<CombatModifier> modifiers_;

	CombatAction();
	virtual ~CombatAction();
};


#endif