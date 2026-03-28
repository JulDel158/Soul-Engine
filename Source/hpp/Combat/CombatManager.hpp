#ifndef COMBAT_MANAGER_HPP
#define COMBAT_MANAGER_HPP
#pragma once

#include <vector>

class CombatCharacter;
class Zone;

class CombatManager
{
private:
	std::vector<CombatCharacter*> player_characters_;
	std::vector<CombatCharacter*> enemy_characters_;
	std::vector<Zone*> zones_;
	
	CombatManager();
	~CombatManager();
	
public:
	CombatManager(const CombatManager&) = delete;
	CombatManager& operator=(const CombatManager&) = delete;
	
	static CombatManager& Instance();
	
	
};

#endif