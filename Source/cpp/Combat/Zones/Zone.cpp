#include "Combat/Zones/Zone.hpp"

Zone::Zone()
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
}

void Zone::OnTurnEnd()
{
}

void Zone::OnTurnCycleStart()
{
}

void Zone::OnTurnCycleEnd()
{
}

void Zone::OnCombatStart()
{
}

void Zone::OnCombatEnd()
{
}
