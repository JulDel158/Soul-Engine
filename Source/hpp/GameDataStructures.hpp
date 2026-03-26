#ifndef GAME_DATA_STRUCTURES_HPP
#define GAME_DATA_STRUCTURES_HPP
#pragma once
#include <cstdint>

enum class EMovementMode : unsigned char
{
	None = 0,
	Walking,
	Running, 
	Swimming
};

enum class EMovementDirection : unsigned char
{
	None = 0,
	Up,
	Down,
	Left,
	Right
};

enum class EPlayerAnimationState : int
{
	None = -1,
	Idle = 0,
	Walking_Up,
	Walking_Down,
	Walking_Left,
	Walking_Right,
};

enum class ECombatModifierType : unsigned int
{
	None = 0,
	Block,
	Speed,
	ActionPoint,
	Damage,
	Healing,
	Wounding,
	SkillCost,
	AttackCost,
	BlockCost,
	MoveCost,
	Movement,
	Range,
	Accuracy
};

enum class ECombatModifierExecutionTime : unsigned char
{
	OnApply = 0,
	TurnStart,
	TurnEnd,
	Ongoing,
	OnSkillUsage,
	OnAttackUsage,
	OnBlockUsage,
	OnDodge,
	OnMove
};

enum class ETargetingType : unsigned char
{
	Self = 0,
	Enemy,
	Friend, // A teammate, but not self
	Party, // Anyone from the caster's party including self
	Any, // Doesn't include zone
	Zone, // The physical zone itself
	ZoneCharacters, // The characters in the zone
	AllButSelf, // This affects everyone excluding the caster
	Everyone // This affects everyone
};

enum class EActionRange : unsigned char
{
	Short = 0,
	Long,
	Any,
	Front = Short,
	Back = Long
};

#endif