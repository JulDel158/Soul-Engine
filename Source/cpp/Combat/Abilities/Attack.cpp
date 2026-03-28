#include "Combat/Abilities/Attack.hpp"

#include "Characters/CombatCharacter.hpp"
#include "GameDataStructures.hpp"

Attack::Attack()
{
	cost_ = 1;
}

void Attack::Execute(std::vector<CombatCharacter*>& enemies, std::vector<CombatCharacter*>& allies)
{
	const CharacterStats ownerStats = owner_->GetStats();
	const bool inRange = IsInRange(range_, owner_->GetCombatPosition());
	int totalDamage = 0;
	bool hitAnyTargets = false;
	// TODO: Check for missing for each enemy using random number generator and owner's accuracy, then send OnMiss event
	for (const auto& enemy : enemies)
	{
		int appliedDamage = ownerStats.damage_;
		hitAnyTargets |= enemy->ApplyDamage(appliedDamage, inRange);
		totalDamage += appliedDamage;
	}
	
	owner_->OnAttack(totalDamage, hitAnyTargets);
}
