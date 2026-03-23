#ifndef TEST_LEVEL_HPP
#define TEST_LEVEL_HPP
#pragma once

#include "Level.hpp"

class PlayerCharacter;
class BackgroundTile;

class TestLevel : public Level
{
public:
	PlayerCharacter* player_;
	BackgroundTile* debug_tiles[5];
	unsigned int player_index_;
	TestLevel();
};

#endif