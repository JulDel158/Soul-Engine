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

#endif