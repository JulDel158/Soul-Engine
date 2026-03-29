#include "Combat/Abilities/Attack.hpp"

#include "Characters/CombatCharacter.hpp"
#include "GameDataStructures.hpp"
#include "Utils/RandomGenerator.hpp"

namespace
{
	// TODO: Move this to some data structure that can be modified externally from the code
	constexpr float LOWEST_ACCURACY_CHANCE = 12.5f;
	constexpr float ACCURACY_CHANCE_PER_LEVEL = 25.0f;
}

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
	RandomGenerator randomGenerator;
	for (const auto& enemy : enemies)
	{
		const float accuracyChance = (ownerStats.accuracy_ == 0) ? LOWEST_ACCURACY_CHANCE : static_cast<float>(ownerStats.accuracy_) * ACCURACY_CHANCE_PER_LEVEL;
		const float roll = randomGenerator.GetRandomFloat(0.0f, 100.0f); // NOLINT

		if (accuracyChance < roll) // We missed this target, call on miss, and continue to the next target
		{
			owner_->OnMiss();
			continue;
		}
		
		int appliedDamage = ownerStats.damage_;
		hitAnyTargets |= enemy->ApplyDamage(appliedDamage, inRange);
		totalDamage += appliedDamage;
	}
	
	owner_->OnAttack(totalDamage, hitAnyTargets);
}
