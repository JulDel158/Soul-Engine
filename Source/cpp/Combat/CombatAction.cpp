#include "Combat/CombatAction.hpp"

CombatAction::CombatAction() : targeting_type_(ETargetingType::Enemy)
{
}

CombatAction::~CombatAction()
{
	modifiers_.clear();
}
