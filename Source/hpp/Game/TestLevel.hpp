#ifndef TEST_LEVEL_HPP
#define TEST_LEVEL_HPP
#pragma once

#include "Level.hpp"

class PlayerCharacter;

class TestLevel : public Level
{
public:
	PlayerCharacter* player_;
	unsigned int player_index_;
	TestLevel();
};

#endif