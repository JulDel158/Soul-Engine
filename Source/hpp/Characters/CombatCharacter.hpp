#ifndef COMBAT_CHARACTER_HPP
#define COMBAT_CHARACTER_HPP
#pragma once

#include "Character.hpp"
#include "GameDataStructures.hpp"
#include "Combat/Statuses/Status.hpp"
#include "Combat/Conditions/Condition.hpp"
#include "Utils/Logger.hpp"

#include "robin_hood_hash/robin_hood.h"

#include <typeindex>
#include <typeinfo>
#include <list>

class HealthComponent;

class CombatCharacter : public Character
{
protected:
	CharacterStats stats_; // These are the base stats of the character, only modified by certain actions/equipment
	CharacterStats status_afflicted_stats_; // These stats are updated at the start of each turn cycle
	HealthComponent* health_component_;
	ECombatPosition combat_position_;
	
	robin_hood::unordered_map<std::type_index, Status*> buffs_;
	robin_hood::unordered_map<std::type_index, Status*> debuffs_;
	robin_hood::unordered_map<ECharacterConditionExecutionTime, std::list<Condition*>> conditions_;
	
public:
	CombatCharacter();
	~CombatCharacter() override = default;
	
	inline HealthComponent* GetHealthComponent() const { return health_component_; }
	
	template <typename Type>
	void ApplyBuffStatus(int count);
	template <typename Type>
	void ApplyDebuffStatus(int count);
	
	
	virtual void OnTurnStart();
	virtual void OnTurnEnd();
	virtual void OnTurnCycleStart();
	virtual void OnTurnCycleEnd();
	virtual void OnCombatStart();
	virtual void OnCombatEnd();
	
	virtual void OnAttack(int hitAmount, bool hit);
	virtual void OnAbility();
	virtual void OnBlock(int blockAmount);
	virtual void OnMove(ECombatPosition destination);
	virtual void OnDodge();
	virtual void OnMiss();
	virtual void OnDamaged(int amount);
	virtual void OnHealed(int amount);
	virtual void OnBuffed(int amount);
	virtual void OnDebuffed(int amount);
	
	CharacterStats GetStats(const bool baseStats=false) const { return baseStats ? stats_ : status_afflicted_stats_; }
	
private:
	void InitComponents();
	void UpdateStatuses();
	void ApplyStatuses();
	void UpdateConditions();
	void TriggerConditions(ECharacterConditionExecutionTime executionTime, int amount);
	void TriggerConditions(ECharacterConditionExecutionTime executionTime);
};

template <typename Type>
void CombatCharacter::ApplyBuffStatus(const int count)
{
	if (!buffs_.contains(typeid(Type)))
	{
		// We must create a buff object
		Type* buff = new Type();
		try
		{
			auto* ptr = dynamic_cast<Status*>(buff);
			ptr->SetStack(count);
			buffs_[typeid(Type)] = ptr;
		}
		catch (...)
		{
			auto log = Logger();
			log.Log(ELogLevel::Error, "CombatCharacter::ApplyBuffStatus: Type: ["+ std::string(typeid(Type).name()) +"] was not a Status!");
		}
		return;
	}
	
	// Otherwise we just add to the current existing one
	buffs_[typeid(Type)]->IncreaseStack(count);
}

template <typename Type>
void CombatCharacter::ApplyDebuffStatus(const int count)
{
	if (!debuffs_.contains(typeid(Type)))
	{
		// We must create a buff object
		Type* debuff = new Type();
		try
		{
			auto* ptr = dynamic_cast<Status*>(debuff);
			ptr->SetStack(count);
			debuffs_[typeid(Type)] = ptr;
		}
		catch (...)
		{
			auto log = Logger();
			log.Log(ELogLevel::Error, "CombatCharacter::ApplyDebuffStatus: Type: ["+ std::string(typeid(Type).name()) +"] was not a Status!");
		}
		return;
	}
	
	// Otherwise we just add to the current existing one
	debuffs_[typeid(Type)]->IncreaseStack(count);
}

#endif
