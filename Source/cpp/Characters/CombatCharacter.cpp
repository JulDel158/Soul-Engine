#include "Characters/CombatCharacter.hpp"

#include "Utils/ResourceManager.hpp"
#include "Utils/Logger.hpp"
#include "EngineDataStructures.hpp"
#include "Combat/Statuses/Status.hpp"
#include "Combat/Conditions/Condition.hpp"
#include "Components/HealthComponent.hpp"


CombatCharacter::CombatCharacter() : 
health_component_(nullptr)
{
	InitComponents();
}

CombatCharacter::~CombatCharacter()
{
	ClearBuffs();
	ClearDebuffs();
	ClearConditions();
}

void CombatCharacter::ClearBuffs()
{
	for (const auto& buff : buffs_)
	{
		delete buff.second;
	}
	buffs_.clear();
}

void CombatCharacter::ClearDebuffs()
{
	for (const auto& debuff : debuffs_)
	{
		delete debuff.second;
	}
	debuffs_.clear();
}

void CombatCharacter::ClearConditions()
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

void CombatCharacter::OnTurnStart()
{
	TriggerConditions(ECharacterConditionExecutionTime::OnTurnStart);
}

void CombatCharacter::OnTurnEnd()
{
	TriggerConditions(ECharacterConditionExecutionTime::OnTurnEnd);
}

void CombatCharacter::OnTurnCycleStart()
{
	TriggerConditions(ECharacterConditionExecutionTime::OnTurnCycleStart);
	ApplyStatuses();
}

void CombatCharacter::OnTurnCycleEnd()
{
	TriggerConditions(ECharacterConditionExecutionTime::OnTurnCycleEnd);
	UpdateConditions();
	UpdateStatuses();
}

void CombatCharacter::OnCombatStart()
{
	TriggerConditions(ECharacterConditionExecutionTime::OnCombatStart);
}

void CombatCharacter::OnCombatEnd()
{
	TriggerConditions(ECharacterConditionExecutionTime::OnCombatEnd);
	// Todo: clear statuses and conditions 
}

void CombatCharacter::OnAttack(const int hitAmount, const bool hit)
{
	for (const auto& condition : conditions_[ECharacterConditionExecutionTime::OnAttack])
	{
		condition->Trigger(*this, hitAmount, hit);
	}
}

void CombatCharacter::OnAbility()
{
	TriggerConditions(ECharacterConditionExecutionTime::OnAbility);
}

void CombatCharacter::OnBlock(const int blockAmount)
{
	TriggerConditions(ECharacterConditionExecutionTime::OnBlock, blockAmount);
}

void CombatCharacter::OnMove(const ECombatPosition destination)
{
	for (const auto& condition : conditions_[ECharacterConditionExecutionTime::OnMove])
	{
		condition->Trigger(*this, destination);
	}
}

void CombatCharacter::OnDodge()
{
	TriggerConditions(ECharacterConditionExecutionTime::OnDodge);
}

void CombatCharacter::OnMiss()
{
	TriggerConditions(ECharacterConditionExecutionTime::OnMiss);
}

void CombatCharacter::OnDamaged(const int amount)
{
	TriggerConditions(ECharacterConditionExecutionTime::OnDamaged, amount);
}

void CombatCharacter::OnHealed(const int amount)
{
	TriggerConditions(ECharacterConditionExecutionTime::OnHealed, amount);
}

void CombatCharacter::OnBuffed(const int amount)
{
	TriggerConditions(ECharacterConditionExecutionTime::OnBuffed, amount);
}

void CombatCharacter::OnDebuffed(const int amount)
{
	TriggerConditions(ECharacterConditionExecutionTime::OnDebuffed, amount);
}

void CombatCharacter::InitComponents()
{
	try
	{
		auto& resourceManager = ResourceManager::Instance();
		unsigned int componentIndex;
		health_component_ = resourceManager.CreateComponent<HealthComponent>(componentIndex, this);
		RegisterComponent(health_component_);
	}
	catch (...)
	{
		auto log = Logger();
		log.Log(ELogLevel::Error, "CombatCharacter::InitializeComponents: Failed to initialize components!");
	}
}

void CombatCharacter::UpdateStatuses()
{
	for (auto i = buffs_.begin(); i != buffs_.end();)
	{
		i->second->OnTurnCycleEnd();
		if (i->second->GetStack() == 0)
		{
			delete i->second;
			i = buffs_.erase(i);
		}
		else
		{
			++i;
		}
	}
	
	for (auto i = debuffs_.begin(); i != debuffs_.end();)
	{
		i->second->OnTurnCycleEnd();
		if (i->second->GetStack() == 0)
		{
			delete i->second;
			i = debuffs_.erase(i);
		}
		else
		{
			++i;
		}
	}
}

void CombatCharacter::ApplyStatuses()
{
	status_afflicted_stats_ = CharacterStats(stats_);
	for (const auto& buff : buffs_)
	{
		buff.second->Modify(status_afflicted_stats_);
	}
	
	for (const auto& debuff : debuffs_)
	{
		debuff.second->Modify(status_afflicted_stats_);
	}
}

void CombatCharacter::UpdateConditions()
{
	for (auto& conditionList : conditions_)
	{
		for (auto it = conditionList.second.begin(); it != conditionList.second.end();)
		{
			if ((*it)->RemoveOnTurnCycleEnd())
			{
				delete (*it);
				it = conditionList.second.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
	
}

void CombatCharacter::TriggerConditions(const ECharacterConditionExecutionTime executionTime, int amount)
{
	for (auto it = conditions_[executionTime].begin(); it != conditions_[executionTime].end();)
	{
		(*it)->Trigger(*this, amount);
		if ((*it)->RemoveOnTurnCycleEnd())
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

void CombatCharacter::TriggerConditions(const ECharacterConditionExecutionTime executionTime)
{
	for (auto& condition : conditions_[executionTime])
	{
		condition->Trigger(*this);
	}
}

namespace {
	// bool CombatCharacter::TakeAttackDamage(const int amount, const  bool inRange, int& appliedDamage,
	// 	const std::vector<CombatModifier>& modifiers)
	// {
	// 	bool result = false;
	// 	bool dodged = false;
	// 	
	// 	if (is_blocking_)
	// 	{
	// 		appliedDamage = amount - GetBlock();
	// 		result = true;
	// 	}
	// 	else
	// 	{
	// 		if (!inRange)
	// 		{
	// 			// TODO: Roll for change using speed
	// 			const float dodgeChance = (GetSpeed() == 0) ? 12.5f : static_cast<float>(GetSpeed()) * 25.0f;
	// 			const float roll = RandomGenerator().GetRandomFloat(0.0f, 100.0f);
	// 			
	// 			dodged = dodgeChance >= roll;
	// 		}
	// 	}
	// 	
	// 	if (dodged)
	// 	{
	// 		appliedDamage = 0;
	// 		result = false;
	// 		if (on_dodge_modifiers_.contains(ECombatModifierType::Block))
	// 	}
	// 	else
	// 	{
	// 		health_component_->Damage(appliedDamage, 0);
	// 		result = true;
	// 	}
	// 	
	// 	return result;
	//}
}
