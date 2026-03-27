#include "Characters/CombatCharacter.hpp"

#include "Utils/ResourceManager.hpp"
#include "Utils/Logger.hpp"
#include "EngineDataStructures.hpp"
#include "Combat/Statuses/Status.hpp"
#include "Components/HealthComponent.hpp"
#include "Utils/RandomGenerator.hpp"


CombatCharacter::CombatCharacter() : 
health_component_(nullptr)
{
	InitComponents();
}

void CombatCharacter::OnTurnStart()
{
}

void CombatCharacter::OnTurnEnd()
{
}

void CombatCharacter::OnTurnCycleStart()
{
	ApplyStatuses();
}

void CombatCharacter::OnTurnCycleEnd()
{
	UpdateStatuses();
}

void CombatCharacter::OnCombatStart()
{
}

void CombatCharacter::OnCombatEnd()
{
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
