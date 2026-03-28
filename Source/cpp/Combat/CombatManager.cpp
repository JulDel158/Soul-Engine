#include "Combat/CombatManager.hpp"

CombatManager::CombatManager()
{
}

CombatManager::~CombatManager()
{
}

CombatManager& CombatManager::Instance()
{
	static CombatManager instance = CombatManager();
	return instance;
}
