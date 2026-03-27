#ifndef GAME_DATA_STRUCTURES_HPP
#define GAME_DATA_STRUCTURES_HPP
#pragma once

#include "glm/common.hpp"

constexpr int SPEED_MAX = 4;
constexpr int ACCURACY_MAX = 5;

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

enum class ETargetingType : unsigned char
{
	Self = 0,
	Enemy,
	Friend, // A teammate, but not self
	PartyMember, // Anyone from the caster's party including self
	Party, // The entire party at once
	EnemyParty, // The entire enemy party at once
	AnyCharacter,
	EveryoneButSelf, // This affects everyone excluding the caster
	Everyone, // This affects everyone
	ZoneCharacters, // All the characters in a zone
	Zone, // A physical zone itself
	PlayerArea, // Both zones on the player's side
	EnemyArea, // Both zones on the enemy's side
	AnyArea, // Either the player or the enemy's area
	AllAreas, // Both the Player and the Enemy's area
};

enum class EActionRange : unsigned char
{
	Short = 0,
	Long = 1,
	Any = 2,
};

enum class ECombatPosition : unsigned char
{
	None = 0,
	FrontPlayer,
	BackPlayer,
	FrontEnemy,
	BackEnemy,
};

enum class ECharacterConditionExecutionTime : unsigned char
{
	OnTurnStart = 0,
	OnTurnEnd,
	OnTurnCycleStart,
	OnTurnCycleEnd,
	OnCombatStart,
	OnCombatEnd,
	
	OnAttack,
	OnAbility,
	OnBlock,
	OnMove,
	OnDodge,
	OnMiss,
	OnDamaged,
	OnHealed,
	OnBuffed,
	OnDebuffed,
};

enum class EZoneConditionExecutionTime : unsigned char
{
	OnTurnStart = 0,
	OnTurnEnd,
	OnTurnCycleStart,
	OnTurnCycleEnd,
	OnCombatStart,
	OnCombatEnd,
};

struct CharacterStats
{
	int damage_;
	int block_;
	int speed_;
	int accuracy_;
	int action_points_;
	
	explicit CharacterStats(const int damage, const int block, const int speed, const int accuracy, const int action_points) :
	damage_(damage), block_(block), speed_(glm::clamp(speed, 0, SPEED_MAX)), accuracy_(glm::clamp(accuracy, 0, ACCURACY_MAX)), action_points_(action_points) {}
	
	CharacterStats() : damage_(0), block_(0), speed_(0), accuracy_(0), action_points_(0) {}
};

#endif