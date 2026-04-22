#include "Combat/Zones/Zone.hpp"
#include "UI/Button.hpp"

Zone::Zone() : 
location_(ECombatPosition::None)
{
	targeting_button_ = new Button();
}

Zone::~Zone()
{
	ClearBuffs();
	ClearDebuffs();
	ClearConditions();
	delete targeting_button_;
}

void Zone::ClearBuffs()
{
	for (const auto& buff : buffs_)
	{
		delete buff.second;
	}
	buffs_.clear();
}

void Zone::ClearDebuffs()
{
	for (const auto& debuff : debuffs_)
	{
		delete debuff.second;
	}
	debuffs_.clear();
}

void Zone::ClearConditions()
{
	for (auto& list : conditions_)
	{
		for (const auto& condition : list.second)
		{
			delete condition;
		}
	}
	conditions_.clear();
}

void Zone::OnTurnStart()
{
	TriggerConditions(EZoneConditionExecutionTime::OnTurnStart);
}

void Zone::OnTurnEnd()
{
	TriggerConditions(EZoneConditionExecutionTime::OnTurnEnd);
}

void Zone::OnTurnCycleStart()
{
	TriggerConditions(EZoneConditionExecutionTime::OnTurnCycleStart);
}

void Zone::OnTurnCycleEnd()
{
	TriggerConditions(EZoneConditionExecutionTime::OnTurnCycleEnd);
}

void Zone::OnCombatStart()
{
	TriggerConditions(EZoneConditionExecutionTime::OnCombatStart);
}

void Zone::OnCombatEnd()
{
	TriggerConditions(EZoneConditionExecutionTime::OnCombatEnd);
}

bool Zone::IsValidTarget(ETargetingType targetType, ECombatPosition zone) const
{
	bool result = false;
	switch (targetType)
	{
	case ETargetingType::CharacterSelection:
	case ETargetingType::Self:
	case ETargetingType::Foe:
	case ETargetingType::Friend:
	case ETargetingType::PartyMember:
	case ETargetingType::Party:
	case ETargetingType::FoeParty:
	case ETargetingType::AnyCharacter:
	case ETargetingType::EveryoneButSelf:
	case ETargetingType::AnyoneButSelf:
	case ETargetingType::Everyone:
	case ETargetingType::ZoneCharacters:
		result = false;
		break;
		
	case ETargetingType::PlayerArea:
		result = location_ == ECombatPosition::BackPlayer || location_ == ECombatPosition::FrontPlayer;
		break;
	case ETargetingType::EnemyArea:
		result = location_ == ECombatPosition::BackEnemy || location_ == ECombatPosition::FrontEnemy;
		break;
		
	case ETargetingType::Zone:
	case ETargetingType::AnyArea:
	case ETargetingType::AllAreas:
		result = true;
		break;
	}
	
	return result;
}

void Zone::TriggerConditions(EZoneConditionExecutionTime executionTime)
{
	for (auto it = conditions_[executionTime].begin(); it != conditions_[executionTime].end();)
	{
		// TODO: Finish this function
		//for all characters
		//(*it)->Trigger(character, amount);
		if ((*it)->RemoveOnTrigger())
		{
			delete (*it);
			it = conditions_[executionTime].erase(it);
		}
		else
		{
			++it;
		}
	}
}
