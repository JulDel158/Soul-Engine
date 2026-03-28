#include "Characters/CombatCharacter.hpp"

#include "Utils/ResourceManager.hpp"
#include "Utils/Logger.hpp"
#include "EngineDataStructures.hpp"
#include "Combat/Statuses/Status.hpp"
#include "Combat/Conditions/Condition.hpp"
#include "Components/HealthComponent.hpp"
#include "Utils/RandomGenerator.hpp"

namespace
{
	constexpr float LOWEST_DODGE_CHANCE = 12.5f;
	constexpr float DODGE_CHANCE_PER_LEVEL = 25.0f;
}

CombatCharacter::CombatCharacter() : 
health_component_(nullptr),
attack_(nullptr)
{
	is_blocking_ = false;
	InitComponents();
}

CombatCharacter::~CombatCharacter()
{
	ClearBuffs();
	ClearDebuffs();
	ClearConditions();
}

bool CombatCharacter::ApplyDamage(int& amount, const bool inRange)
{
	bool dodged = false;
	int appliedDamage = 0;
		
	if (is_blocking_)
	{
		appliedDamage = amount - status_afflicted_stats_.block_;
		OnBlock(status_afflicted_stats_.block_);
	}
	else if (!inRange)
	{
		const float dodgeChance = (status_afflicted_stats_.speed_ == 0) ? LOWEST_DODGE_CHANCE : static_cast<float>(status_afflicted_stats_.speed_) * DODGE_CHANCE_PER_LEVEL;
		const float roll = RandomGenerator().GetRandomFloat(0.0f, 100.0f);
		dodged = dodgeChance >= roll;
	}
	
	if (dodged)
	{
		appliedDamage = 0;
		OnDodge();
	}
	else
	{
		health_component_->Damage(appliedDamage, 0);
		OnDamaged(appliedDamage);
	}
	
	amount = appliedDamage;
	
	return !dodged;
}

void CombatCharacter::ApplyHealing(const int amount, const bool inRange)
{
	int total = amount;
	health_component_->Heal(total, 0);
	
	OnHealed(total);
}

void CombatCharacter::Move()
{
	switch (combat_position_)
	{
	case ECombatPosition::None:
		{
			auto log = Logger();
			log.Log(ELogLevel::Warning, "CombatCharacter::Move: combat_position_ was none?!?!");
			break;
		}
	case ECombatPosition::FrontPlayer:
		combat_position_ = ECombatPosition::BackPlayer;
		break;
	case ECombatPosition::BackPlayer:
		combat_position_ = ECombatPosition::FrontPlayer;
		break;
	case ECombatPosition::FrontEnemy:
		combat_position_ = ECombatPosition::BackEnemy;
		break;
	case ECombatPosition::BackEnemy:
		combat_position_ = ECombatPosition::FrontEnemy;
		break;
	}
	OnMove(combat_position_);
}

void CombatCharacter::Block()
{
	if (status_afflicted_stats_.can_block_)
	{
		is_blocking_ = true;
	}
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
	is_blocking_ = false;
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

void CombatCharacter::OnBuffed(const std::type_index type, const int amount)
{
	TriggerConditions(ECharacterConditionExecutionTime::OnBuffed, type,amount);
}

void CombatCharacter::OnDebuffed(const std::type_index type, const int amount)
{
	TriggerConditions(ECharacterConditionExecutionTime::OnDebuffed, type,amount);
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

void CombatCharacter::TriggerConditions(const ECharacterConditionExecutionTime executionTime)
{
	for (auto it = conditions_[executionTime].begin(); it != conditions_[executionTime].end();)
	{
		(*it)->Trigger(*this);
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

void CombatCharacter::TriggerConditions(const ECharacterConditionExecutionTime executionTime, const std::type_index type,
	const int amount)
{
	for (auto it = conditions_[executionTime].begin(); it != conditions_[executionTime].end();)
	{
		(*it)->Trigger(*this, type , amount);
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
