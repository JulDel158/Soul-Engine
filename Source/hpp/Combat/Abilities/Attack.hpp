#ifndef ATTACK_HPP
#define ATTACK_HPP
#pragma once

#include "Ability.hpp"

#include <vector>

class Attack : public Ability
{
public:
	Attack();
	~Attack() override = default;
	
	void Execute(std::vector<CombatCharacter*>& enemies, std::vector<CombatCharacter*>& allies) override;
};

#endif