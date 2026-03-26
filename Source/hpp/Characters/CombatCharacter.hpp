#ifndef COMBAT_CHARACTER_HPP
#define COMBAT_CHARACTER_HPP
#pragma once

#include "Character.hpp"
#include "Combat/CombatAction.hpp"
#include "Combat/CombatModifier.hpp"
#include "GameDataStructures.hpp"
#include "robin_hood_hash/robin_hood.h"

#include <vector>

class HealthComponent;

class CombatCharacter : public Character
{
protected:
	HealthComponent* health_component_;
	CombatAction attack_action_;
	CombatAction block_action_;
	CombatAction move_action_;
	std::vector<CombatAction> skills_;
	
	robin_hood::unordered_map<ECombatModifierType ,std::vector<CombatModifier>> ongoing_modifiers_;
	robin_hood::unordered_map<ECombatModifierType ,std::vector<CombatModifier>> turn_start_modifiers_;
	robin_hood::unordered_map<ECombatModifierType ,std::vector<CombatModifier>> turn_end_modifiers_;
	robin_hood::unordered_map<ECombatModifierType ,std::vector<CombatModifier>> on_skill_usage_modifiers_;
	robin_hood::unordered_map<ECombatModifierType ,std::vector<CombatModifier>> on_attack_usage_modifiers_;
	robin_hood::unordered_map<ECombatModifierType ,std::vector<CombatModifier>> on_block_usage_modifiers_;
	robin_hood::unordered_map<ECombatModifierType ,std::vector<CombatModifier>> on_dodge_modifiers_;
	robin_hood::unordered_map<ECombatModifierType ,std::vector<CombatModifier>> on_move_modifiers_;
	
	int speed_;
	int block_;
	int action_points_;
	int max_action_points_;
	EActionRange position_;
	bool is_blocking_;
	
public:
	CombatCharacter();
	~CombatCharacter() override = default;
	
	void OnTurnStart();
	void OnTurnEnd();
	
	inline void SetActionPoints(const int points) { action_points_ = points; }
	
	inline HealthComponent* GetHealthComponent() const { return health_component_; }
	int GetSpeed() const;
	int GetBlock() const;
	inline int GetActionPoints() const { return action_points_; }
	
	const std::vector<CombatAction>& GetSkill() const { return skills_; }
	CombatAction GetAttackAction() const { return attack_action_; }
	CombatAction GetBlockAction() const { return block_action_; }
	CombatAction GetMoveAction() const { return move_action_; }
	
	void ApplyModifiers(const std::vector<CombatModifier>& modifiers);
	
	bool TakeDamage(int amount, bool inRange, int& appliedDamage,const std::vector<CombatModifier>& modifiers);
	
private:
	void InitComponents();
	
protected:
	virtual void ResolveModifier(CombatModifier modifier);
	
	// TODO: Add combat character base attributes
	// TODO: Add enum for health component and initialize in Resource Manager
};

#endif
