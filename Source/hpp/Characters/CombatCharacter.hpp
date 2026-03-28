#ifndef COMBAT_CHARACTER_HPP
#define COMBAT_CHARACTER_HPP
#pragma once

#include "robin_hood_hash/robin_hood.h"

#include "Character.hpp"
#include "GameDataStructures.hpp"
#include "Combat/Statuses/Status.hpp"
#include "Combat/Conditions/Condition.hpp"
#include "Combat/Abilities/Ability.hpp"
#include "Utils/Logger.hpp"

#include <typeindex>
#include <typeinfo>
#include <list>
#include <vector>

class HealthComponent;

class CombatCharacter : public Character
{
protected:
	CharacterStats stats_; // These are the base stats of the character, only modified by certain actions/equipment
	CharacterStats status_afflicted_stats_; // These stats are updated at the start of each turn cycle
	HealthComponent* health_component_;
	Ability* attack_;
	std::vector<Ability*> abilities_;
	
	ECombatPosition combat_position_;
	bool is_blocking_;
	
	robin_hood::unordered_map<std::type_index, Status*> buffs_;
	robin_hood::unordered_map<std::type_index, Status*> debuffs_;
	robin_hood::unordered_map<ECharacterConditionExecutionTime, std::list<Condition*>> conditions_;
	
public:
	CombatCharacter();
	~CombatCharacter() override;
	
	template <typename Type>
	void ApplyBuffStatus(int count);
	template <typename Type>
	void ApplyDebuffStatus(int count);
	template <typename Type>
	void ApplyTrigger(ECharacterConditionExecutionTime executionTime, CombatCharacter& caster);
	
	bool ApplyDamage(int& amount, bool inRange);
	void ApplyHealing(int amount, bool inRange);
	
	void Move();
	void Block();
	
	void ClearBuffs();
	void ClearDebuffs();
	void ClearConditions();
	
	virtual void OnTurnStart();
	virtual void OnTurnEnd();
	virtual void OnTurnCycleStart();
	virtual void OnTurnCycleEnd();
	virtual void OnCombatStart();
	virtual void OnCombatEnd();
	
	virtual void OnAttack(int hitAmount, bool hit); // TODO: CALL
	virtual void OnAbility(); // TODO: CALL
	virtual void OnBlock(int blockAmount);
	virtual void OnMove(ECombatPosition destination);
	virtual void OnDodge();
	virtual void OnMiss(); // TODO: CALL
	virtual void OnDamaged(int amount);
	virtual void OnHealed(int amount);
	virtual void OnBuffed(std::type_index type, int amount);
	virtual void OnDebuffed(std::type_index type, int amount);
	
	inline CharacterStats GetStats(const bool baseStats=false) const { return baseStats ? stats_ : status_afflicted_stats_; }
	inline HealthComponent* GetHealthComponent() const { return health_component_; }
	inline ECombatPosition GetCombatPosition() const { return combat_position_; }
	
	inline Ability* GetAttack() const { return attack_; }
	inline const std::vector<Ability*>& GetAbilities() const { return abilities_; }
	inline Ability* GetAbility(const unsigned int index) const { if (!abilities_.empty() && abilities_.size() > index) return abilities_[index]; return nullptr; }
	
private:
	void InitComponents();
	void UpdateStatuses();
	void ApplyStatuses();
	void UpdateConditions();
	void TriggerConditions(ECharacterConditionExecutionTime executionTime, int amount);
	void TriggerConditions(ECharacterConditionExecutionTime executionTime);
	void TriggerConditions(ECharacterConditionExecutionTime executionTime, std::type_index type, int amount);
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
			OnBuffed(typeid(Type), count);
		}
		catch (...)
		{
			auto log = Logger();
			log.Log(ELogLevel::Error, "CombatCharacter::ApplyBuffStatus: Type: ["+ std::string(typeid(Type).name()) +"] was not a Status!");
			delete buff;
		}
		return;
	}
	
	// Otherwise we just add to the current existing one
	buffs_[typeid(Type)]->IncreaseStack(count);
	OnBuffed(typeid(Type), count);
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
			OnDebuffed(typeid(Type), count);
		}
		catch (...)
		{
			auto log = Logger();
			log.Log(ELogLevel::Error, "CombatCharacter::ApplyDebuffStatus: Type: ["+ std::string(typeid(Type).name()) +"] was not a Status!");
			delete debuff;
		}
		return;
	}
	
	// Otherwise we just add to the current existing one
	debuffs_[typeid(Type)]->IncreaseStack(count);
	OnDebuffed(typeid(Type), count);
}

template <typename Type>
void CombatCharacter::ApplyTrigger(const ECharacterConditionExecutionTime executionTime, CombatCharacter& caster)
{
	Type* type = new Type();
	try
	{
		auto condition = dynamic_cast<Condition*>(type);
		condition->SetCaster(caster);
		if ( condition->IsStackable()) // No need to check for instances, just add it to the list
		{
			conditions_[executionTime].push_back(condition);
		}
		else
		{
			bool containsCondition = false;
			for (const auto& ptr : conditions_[executionTime])
			{
				if (ptr->GetName() == condition->GetName())
				{
					containsCondition = true;
					break;
				}
			}
			
			if (!containsCondition)
			{
				conditions_[executionTime].push_back(condition);
			}
			else
			{
				// The object won't be added to the list, therefore delete it
				condition = nullptr;
				delete type;
			}
		}
	}
	catch (...)
	{
		auto log = Logger();
		log.Log(ELogLevel::Error, "CombatCharacter::ApplyTrigger: failed to create condition [" + std::string(typeid(Type).name()) + "]!");
		delete type;
	}
}

#endif
