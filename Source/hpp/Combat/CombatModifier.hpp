#ifndef COMBAT_MODIFIER_HPP
#define COMBAT_MODIFIER_HPP
#pragma once

#include "GameDataStructures.hpp"

#include <string>

class CombatModifier
{
public:
	CombatModifier();
	explicit CombatModifier(ECombatModifierType type, ECombatModifierExecutionTime executionTime, int count);
	
	std::string name_;
	ECombatModifierType type_;
	ECombatModifierExecutionTime execution_time_;
	int stacks_;
	int data_int_;
	unsigned int data_uint_;
	float data_float_;
	bool data_bool_;
};

#endif
