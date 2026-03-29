#include "Combat/CombatManager.hpp"

#include "EngineDataStructures.hpp"
#include "Components/SpriteComponent.hpp"
#include "Utils/ResourceManager.hpp"

namespace
{
	constexpr unsigned int ATTACK_BUTTON = 0;
	constexpr unsigned int SKILL_BUTTON = 1;
	constexpr unsigned int BLOCK_BUTTON = 2;
	constexpr unsigned int MOVE_BUTTON = 3;
}

CombatManager::CombatManager() : 
zones_{nullptr}
{
	InitializeAbilityButtons();
	InitializeSkillButtons();
	InitializePlayerTargetingButtons();
	InitializeEnemyTargetingButtons();
}

CombatManager::~CombatManager()
{
}

CombatManager& CombatManager::Instance()
{
	static CombatManager instance = CombatManager();
	return instance;
}

void CombatManager::InitializeAbilityButtons()
{
	ResourceManager& resourceManager = ResourceManager::Instance();
	// TODO: Initialize button data
	Button& attButton = ability_buttons_[ATTACK_BUTTON];
	Button& skillButton = ability_buttons_[SKILL_BUTTON];
	Button& blockButton = ability_buttons_[BLOCK_BUTTON];
	Button& moveButton = ability_buttons_[MOVE_BUTTON];
	
	attButton.SetLabel("Attack");
	skillButton.SetLabel("Skills");
	blockButton.SetLabel("Block");
	moveButton.SetLabel("Move");
	
	attButton.GetSpriteComponent()->SetDefaultTexture(resourceManager.GetTexture2D(ESpriteKey::Button1));
	skillButton.GetSpriteComponent()->SetDefaultTexture(resourceManager.GetTexture2D(ESpriteKey::Button1));
	blockButton.GetSpriteComponent()->SetDefaultTexture(resourceManager.GetTexture2D(ESpriteKey::Button1));
	moveButton.GetSpriteComponent()->SetDefaultTexture(resourceManager.GetTexture2D(ESpriteKey::Button1));
	
	attButton.AddNeighbor(moveButton, EWidgetNeighbor::Up);
	attButton.AddNeighbor(skillButton, EWidgetNeighbor::Down);
	
	skillButton.AddNeighbor(attButton, EWidgetNeighbor::Up);
	skillButton.AddNeighbor(blockButton, EWidgetNeighbor::Down);
	
	blockButton.AddNeighbor(skillButton, EWidgetNeighbor::Up);
	blockButton.AddNeighbor(moveButton, EWidgetNeighbor::Down);
	
	moveButton.AddNeighbor(blockButton, EWidgetNeighbor::Up);
	moveButton.AddNeighbor(attButton, EWidgetNeighbor::Down);
	
	// TODO: Set texture of buttons
}

void CombatManager::InitializeSkillButtons()
{
	
}

void CombatManager::InitializePlayerTargetingButtons()
{
	
}

void CombatManager::InitializeEnemyTargetingButtons()
{
	
}
