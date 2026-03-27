#include "Combat/Conditions/Condition.hpp"

#include "Characters/CombatCharacter.hpp"
#include "Utils/Logger.hpp"
#include "EngineDataStructures.hpp"

Condition::Condition() :
caster_(nullptr),
name_("Condition"),
description_("Default description...")
{
}

Condition::~Condition()
{
	caster_ = nullptr;
}

void Condition::Trigger(CombatCharacter& target)
{
	auto log = Logger();
	log.Log(ELogLevel::Warning, "Condition::Trigger(CombatCharacter&) called but base implementation executed!!");
}

void Condition::Trigger(CombatCharacter& target, int data)
{
	auto log = Logger();
	log.Log(ELogLevel::Warning, "Condition::Trigger(CombatCharacter&,int) called but base implementation executed!!");
}

void Condition::Trigger(CombatCharacter& target, int data, bool flag)
{
	auto log = Logger();
	log.Log(ELogLevel::Warning, "Condition::Trigger(CombatCharacter&,int,bool) called but base implementation executed!!");
}

void Condition::Trigger(CombatCharacter& target, ECombatPosition destination)
{
	auto log = Logger();
	log.Log(ELogLevel::Warning, "Condition::Trigger(CombatCharacter&,ECombatPosition) called but base implementation executed!!");
}

bool Condition::IsStackable() const
{
	return true;
}

bool Condition::RemoveOnTurnCycleEnd() const
{
	return true;
}

bool Condition::RemoveOnTrigger() const
{
	return true;
}
