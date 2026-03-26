#include "Characters/CombatCharacter.hpp"

#include "Utils/ResourceManager.hpp"
#include "Utils/Logger.hpp"
#include "EngineDataStructures.hpp"
#include "Components/HealthComponent.hpp"
#include "Utils/RandomGenerator.hpp"

CombatCharacter::CombatCharacter() : 
health_component_(nullptr), 
speed_(0), 
block_(0), 
action_points_(0),
position_(EActionRange::Front),
is_blocking_(false)
{
	InitComponents();
}

void CombatCharacter::OnTurnStart()
{
	action_points_ = max_action_points_;
}

void CombatCharacter::OnTurnEnd()
{
	// TODO: Update modifiers
}

int CombatCharacter::GetSpeed() const
{
	return speed_; // TODO: apply modifiers
}

int CombatCharacter::GetBlock() const
{
	int amount = block_;
	if (ongoing_modifiers_.contains(ECombatModifierType::Block))
	{
		for (const auto& modifier : ongoing_modifiers_.at(ECombatModifierType::Block))
		{
			amount += modifier.data_int_;
		}
	}
	return amount;
}

void CombatCharacter::ApplyModifiers(const std::vector<CombatModifier>& modifiers)
{
	for (const auto& modifier : modifiers)
	{	
		switch (modifier.execution_time_)
		{
		case ECombatModifierExecutionTime::OnApply: // We must resolve the action now
			ResolveModifier(modifier);
			break;
		case ECombatModifierExecutionTime::TurnStart:
			break;
		case ECombatModifierExecutionTime::TurnEnd:
			break;
		case ECombatModifierExecutionTime::Ongoing:
			break;
		case ECombatModifierExecutionTime::OnSkillUsage:
			break;
		case ECombatModifierExecutionTime::OnAttackUsage:
			break;
		case ECombatModifierExecutionTime::OnBlockUsage:
			break;
		case ECombatModifierExecutionTime::OnDodge:
			break;
		case ECombatModifierExecutionTime::OnMove:
			break;
		}
	}
}

bool CombatCharacter::TakeDamage(const int amount, const  bool inRange, int& appliedDamage,
	const std::vector<CombatModifier>& modifiers)
{
	bool result = false;
	bool dodged = false;
	
	if (is_blocking_)
	{
		appliedDamage = amount - GetBlock();
		result = true;
	}
	else
	{
		if (!inRange)
		{
			// TODO: Roll for change using speed
			const float dodgeChance = (GetSpeed() == 0) ? 12.5f : static_cast<float>(GetSpeed()) * 25.0f;
			const float roll = RandomGenerator().GetRandomFloat(0.0f, 100.0f);
			
			dodged = dodgeChance >= roll;
		}
	}
	
	if (dodged)
	{
		appliedDamage = 0;
		result = false;
	}
	else
	{
		health_component_->Damage(appliedDamage, 0);
		result = true;
	}
	
	return result;
}

void CombatCharacter::InitComponents()
{
	try
	{
		auto& resourceManager = ResourceManager::Instance();
		unsigned int componentIndex;
		health_component_ = dynamic_cast<HealthComponent*>(resourceManager.CreateComponent(EComponentClassType::Base,componentIndex, this));
		RegisterComponent(health_component_);
		
	}
	catch (...)
	{
		auto log = Logger();
		log.Log(ELogLevel::Error, "CombatCharacter::InitializeComponents: Failed to initialize components!");
	}
}

void CombatCharacter::ResolveModifier(CombatModifier modifier)
{
	switch (modifier.type_) // NOLINT
	{
	case ECombatModifierType::Movement:
		break;
	default:
		break;
	}
}
