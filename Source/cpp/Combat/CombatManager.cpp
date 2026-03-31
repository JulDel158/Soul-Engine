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
	zones_{nullptr}, 
	is_player_turn_(false), 
	selected_character_index_(0)
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
	// 1 2
	// 3 4
	// 5 6
	ResourceManager& resourceManager = ResourceManager::Instance();
	
	Button& button1 = skill_buttons_[0];
	Button& button2 = skill_buttons_[1];
	Button& button3 = skill_buttons_[2];
	Button& button4 = skill_buttons_[3];
	Button& button5 = skill_buttons_[4];
	Button& button6 = skill_buttons_[5];
	
	button1.AddNeighbor(button5, EWidgetNeighbor::Up);
	button1.AddNeighbor(button3, EWidgetNeighbor::Down);
	button1.AddNeighbor(button2, EWidgetNeighbor::Left);
	button1.AddNeighbor(button2, EWidgetNeighbor::Right);
	
	button2.AddNeighbor(button6, EWidgetNeighbor::Up);
	button2.AddNeighbor(button4, EWidgetNeighbor::Down);
	button2.AddNeighbor(button1, EWidgetNeighbor::Left);
	button2.AddNeighbor(button1, EWidgetNeighbor::Right);
	
	button3.AddNeighbor(button1, EWidgetNeighbor::Up);
	button3.AddNeighbor(button5, EWidgetNeighbor::Down);
	button3.AddNeighbor(button4, EWidgetNeighbor::Left);
	button3.AddNeighbor(button4, EWidgetNeighbor::Right);
	
	button4.AddNeighbor(button2, EWidgetNeighbor::Up);
	button4.AddNeighbor(button6, EWidgetNeighbor::Down);
	button4.AddNeighbor(button3, EWidgetNeighbor::Left);
	button4.AddNeighbor(button3, EWidgetNeighbor::Right);
	
	button5.AddNeighbor(button3, EWidgetNeighbor::Up);
	button5.AddNeighbor(button1, EWidgetNeighbor::Down);
	button5.AddNeighbor(button6, EWidgetNeighbor::Left);
	button5.AddNeighbor(button6, EWidgetNeighbor::Right);
	
	button6.AddNeighbor(button4, EWidgetNeighbor::Up);
	button6.AddNeighbor(button2, EWidgetNeighbor::Down);
	button6.AddNeighbor(button5, EWidgetNeighbor::Left);
	button6.AddNeighbor(button5, EWidgetNeighbor::Right);
	
	for (int i = 0; i < 6; ++i)
	{
		skill_buttons_[i].SetLabel("Skill " + std::to_string(i+1));
		skill_buttons_[i].SetLabelBaseColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		skill_buttons_[i].SetSize(glm::vec2(160.0f, 40.0f));
		skill_buttons_[i].SetScale(glm::vec2(0.5f));
		skill_buttons_[i].GetSpriteComponent()->SetDefaultTexture(resourceManager.GetTexture2D(ESpriteKey::Button1));
		ui_panel_.AddWidget(skill_buttons_[i]);
	}
	button1.SetPosition(glm::vec2(250.0f, 250.0f));
	button2.SetPosition(glm::vec2(420.0f, 250.0f));
	button3.SetPosition(glm::vec2(250.0f, 300.0f));
	button4.SetPosition(glm::vec2(420.0f, 300.0f));
	button5.SetPosition(glm::vec2(250.0f, 350.0f));
	button6.SetPosition(glm::vec2(420.0f, 350.0f));
	
	button1.BindLeftClick([](){std::cout << "Skill button: 1 pressed!!\n";});
	button2.BindLeftClick([](){std::cout << "Skill button: 2 pressed!!\n";});
	button3.BindLeftClick([](){std::cout << "Skill button: 3 pressed!!\n";});
	button4.BindLeftClick([](){std::cout << "Skill button: 4 pressed!!\n";});
	button5.BindLeftClick([](){std::cout << "Skill button: 5 pressed!!\n";});
	button6.BindLeftClick([](){std::cout << "Skill button: 6 pressed!!\n";});
}

void CombatManager::InitializePlayerTargetingButtons()
{
	// TODO:
}

void CombatManager::InitializeEnemyTargetingButtons()
{
	
}
