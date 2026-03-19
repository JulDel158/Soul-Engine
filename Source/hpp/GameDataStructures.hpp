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

#endif