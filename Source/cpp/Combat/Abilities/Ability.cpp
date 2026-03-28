#include "Combat/Abilities/Ability.hpp"

#include "Characters/CombatCharacter.hpp"
#include "Utils/Logger.hpp"
#include "EngineDataStructures.hpp"

Ability::Ability() :
	cost_(0),
	owner_(nullptr),
	targeting_type_(ETargetingType::Enemy), 
	range_(EActionRange::Any)
{
}

Ability::~Ability()
{
	owner_ = nullptr;
}

void Ability::Execute(std::vector<CombatCharacter&>& enemies, std::vector<CombatCharacter&>& allies)
{
	auto log = Logger();
	log.Log(ELogLevel::Warning, "Ability::Execute base implementation called!");
}
