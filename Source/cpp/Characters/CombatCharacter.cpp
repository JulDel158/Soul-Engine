#include "Characters/CombatCharacter.hpp"

#include "Utils/ResourceManager.hpp"
#include "Utils/Logger.hpp"
#include "EngineDataStructures.hpp"
#include "Components/HealthComponent.hpp"
#include "Utils/RandomGenerator.hpp"

CombatCharacter::CombatCharacter() : 
health_component_(nullptr)
{
	InitComponents();
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