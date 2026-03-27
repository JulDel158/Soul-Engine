#ifndef STATUS_HPP
#define STATUS_HPP
#pragma once

#include "glm/common.hpp"

#include "GameDataStructures.hpp"

#include <string>

// The base class for buffs and debuffs, these can be applied to the characters as well as the zones
class Status
{
protected:
	int stack_;
	int cost_per_cycle_;
	int max_;
	std::string name_;
	std::string description_;
	
public:
	Status();
	virtual ~Status() = default;
	
	int GetStack() const { return stack_; }
	int GetCostPerCycle() const { return cost_per_cycle_; }
	int GetMax() const { return max_; }
	std::string GetName() const { return name_; }
	std::string GetDescription() const { return description_; }
	
	inline void IncreaseStack(const int value)	{ stack_ = glm::clamp(stack_ + value, 0, max_); }
	inline void SetStack(const int value)		{ stack_ = glm::clamp(value, 0, max_); }
	inline void SetCostPerCycle(const int cost)	{ cost_per_cycle_ = cost; }
	inline void SetMax(const int value)			{ max_ = value; SetStack(stack_); }
	
	virtual void OnTurnCycleEnd();
	virtual void Modify(CharacterStats& targetStats)=0; 
};
#endif
