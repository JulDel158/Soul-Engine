#ifndef ZONE_HPP
#define ZONE_HPP
#pragma once

#include "robin_hood_hash/robin_hood.h"

#include "Combat/Conditions/Condition.hpp"
#include "Combat/Statuses/Status.hpp"
#include "Characters/CombatCharacter.hpp"
#include "EngineDataStructures.hpp"
#include "GameDataStructures.hpp"
#include "Utils/Logger.hpp"

#include <typeindex>
#include <typeinfo>

class Zone
{
	robin_hood::unordered_map<std::type_index, Status*> buffs_;
	robin_hood::unordered_map<std::type_index, Status*> debuffs_;
	robin_hood::unordered_map<EZoneConditionExecutionTime, std::list<Condition*>> conditions_;
	ECombatPosition location_;
	
public:
	Zone();
	virtual ~Zone();
	
	template <typename Type>
	void ApplyBuffStatus(int count);
	template <typename Type>
	void ApplyDebuffStatus(int count);
	template <typename Type>
	void ApplyTrigger(EZoneConditionExecutionTime executionTime, CombatCharacter& caster);
	
	void ClearBuffs();
	void ClearDebuffs();
	void ClearConditions();
	
	// TODO: implement these functions
	virtual void OnTurnStart();
	virtual void OnTurnEnd();
	virtual void OnTurnCycleStart();
	virtual void OnTurnCycleEnd();
	virtual void OnCombatStart();
	virtual void OnCombatEnd();
	
private:
	void TriggerConditions(EZoneConditionExecutionTime executionTime);
};

template <typename Type>
void Zone::ApplyBuffStatus(int count)
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
			delete buff;
		}
		return;
	}
	
	// Otherwise we just add to the current existing one
	buffs_[typeid(Type)]->IncreaseStack(count);
}

template <typename Type>
void Zone::ApplyDebuffStatus(int count)
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
			delete debuff;
		}
		return;
	}
	
	// Otherwise we just add to the current existing one
	debuffs_[typeid(Type)]->IncreaseStack(count);
}

template <typename Type>
void Zone::ApplyTrigger(const EZoneConditionExecutionTime executionTime, CombatCharacter& caster)
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
		log.Log(ELogLevel::Error, "Zone::ApplyTrigger: failed to create condition [" + std::string(typeid(Type).name()) + "]!");
		delete type;
	}
}

#endif
