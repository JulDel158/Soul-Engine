#ifndef CONDITION_HPP
#define CONDITION_HPP
#pragma once

#include "GameDataStructures.hpp"

#include <string>

class CombatCharacter;

// An effect that triggers after some specific action 
class Condition
{
protected:
	CombatCharacter* caster_;
	std::string name_;
	std::string description_;
	
public:
	Condition();
	virtual ~Condition();
	
	virtual void Trigger(CombatCharacter& target);
	virtual void Trigger(CombatCharacter& target, int data);
	virtual void Trigger(CombatCharacter& target, int data, bool flag);
	virtual void Trigger(CombatCharacter& target, ECombatPosition destination);
	virtual bool IsStackable() const;
	virtual bool RemoveOnTurnCycleEnd() const;
	
	std::string GetName() const { return name_;}
	std::string GetDescription() const { return description_;}
};

#endif