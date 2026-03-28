#include "Combat/Zones/Zone.hpp"

Zone::Zone() : 
location_(ECombatPosition::None)
{
}

Zone::~Zone()
{
	ClearBuffs();
	ClearDebuffs();
	ClearConditions();
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
