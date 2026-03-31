#include "Combat/CombatManager.hpp"

#include "EngineDataStructures.hpp"
#include "Components/SpriteComponent.hpp"
#include "Utils/ResourceManager.hpp"

#include <iostream>

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
}

CombatManager::~CombatManager()
{
}

CombatManager& CombatManager::Instance()
{
	static CombatManager instance = CombatManager();
	return instance;
}

void CombatManager::Initialize()
{
	InitializeAbilityButtons();
	InitializeSkillButtons();
	InitializePlayerTargetingButtons();
	InitializeEnemyTargetingButtons();
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
	
	constexpr glm::vec4 textColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	attButton.SetLabelBaseColor(textColor);
	skillButton.SetLabelBaseColor(textColor);
	blockButton.SetLabelBaseColor(textColor);
	moveButton.SetLabelBaseColor(textColor);
	
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
	
	attButton.SetPosition(glm::vec2(40.0f, 50.0f));
	skillButton.SetPosition(glm::vec2(40.0f, 100.0f));
	blockButton.SetPosition(glm::vec2(40.0f, 150.0f));
	moveButton.SetPosition(glm::vec2(40.0f, 200.0f));
	
	constexpr auto buttonSize = glm::vec2(80.f, 40.0f);
	attButton.SetSize(buttonSize);
	skillButton.SetSize(buttonSize);
	blockButton.SetSize(buttonSize);
	moveButton.SetSize(buttonSize);
	
	constexpr auto buttonScale = glm::vec2(0.5f);
	attButton.SetScale(buttonScale);
	skillButton.SetScale(buttonScale);
	blockButton.SetScale(buttonScale);
	moveButton.SetScale(buttonScale);
	
	attButton.BindLeftClick([](){ std::cout << "attack button clicked!\n"; });
	skillButton.BindLeftClick([](){ std::cout << "skill button clicked!\n"; });
	blockButton.BindLeftClick([](){ std::cout << "block button clicked!\n"; });
	moveButton.BindLeftClick([](){ std::cout << "move button clicked!\n"; });
	skillButton.BindSelected([](){ std::cout << "!!skill button selected!!\n"; });
	
	ui_panel_.AddWidget(attButton);
	ui_panel_.AddWidget(skillButton);
	ui_panel_.AddWidget(blockButton);
	ui_panel_.AddWidget(moveButton);
}

void CombatManager::InitializeSkillButtons()
{
	// TODO: 
}

void CombatManager::InitializePlayerTargetingButtons()
{
	// TODO:
}

void CombatManager::InitializeEnemyTargetingButtons()
{
	
}
