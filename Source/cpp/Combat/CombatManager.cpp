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
	
	constexpr unsigned int ABILITY_BUTTON_COUNT = 4;
	constexpr unsigned int SKILL_BUTTON_COUNT = 6;
	constexpr unsigned int PLAYER_TARGETS_BUTTON_COUNT = 4;
	constexpr unsigned int ENEMY_TARGETS_BUTTON_COUNT = 7;
	constexpr unsigned int ZONE_TARGETS_BUTTON_COUNT = 4;
}

CombatManager::CombatManager() :
	zones_{nullptr}, 
	is_player_turn_(false), 
	selected_character_index_(0)
{
	player_characters_.reserve(4);
	enemy_characters_.reserve(7);
	ui_panel_ = new Panel();
	
	ability_buttons_ = new Button[ABILITY_BUTTON_COUNT];
	skill_buttons_ = new Button[SKILL_BUTTON_COUNT];
	player_targeting_buttons_ = new Button[PLAYER_TARGETS_BUTTON_COUNT];
	enemy_targeting_buttons_ = new Button[ENEMY_TARGETS_BUTTON_COUNT];
	zone_targeting_buttons_ = new Button[ZONE_TARGETS_BUTTON_COUNT];
}

CombatManager::~CombatManager()
{
	delete ui_panel_;
	delete[] ability_buttons_;
	delete[] skill_buttons_;
	delete[] player_targeting_buttons_;
	delete[] enemy_targeting_buttons_;
	delete[] zone_targeting_buttons_;
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
	InitializeZoneTargetingButtons();
	
	//ui_panel_->SetActive(false);
	//ui_panel_->SetVisible(false);
}

void CombatManager::BeginCombat()
{
}

void CombatManager::EndCombat()
{
}

void CombatManager::InitializeAbilityButtons()
{
	ResourceManager& resourceManager = ResourceManager::Instance();
	// TODO: Initialize button data
	Button& button1 = ability_buttons_[ATTACK_BUTTON];	//NOLINT
	Button& button2 = ability_buttons_[SKILL_BUTTON];	//NOLINT
	Button& button3 = ability_buttons_[BLOCK_BUTTON];	//NOLINT
	Button& button4 = ability_buttons_[MOVE_BUTTON];	//NOLINT
	
	button1.SetLabel("Attack");
	button2.SetLabel("Skills");
	button3.SetLabel("Block");
	button4.SetLabel("Move");
	
	constexpr glm::vec4 textColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	button1.SetLabelBaseColor(textColor);
	button2.SetLabelBaseColor(textColor);
	button3.SetLabelBaseColor(textColor);
	button4.SetLabelBaseColor(textColor);
	
	button1.GetSpriteComponent()->SetDefaultTexture(resourceManager.GetTexture2D(ESpriteKey::Button1));
	button2.GetSpriteComponent()->SetDefaultTexture(resourceManager.GetTexture2D(ESpriteKey::Button1));
	button3.GetSpriteComponent()->SetDefaultTexture(resourceManager.GetTexture2D(ESpriteKey::Button1));
	button4.GetSpriteComponent()->SetDefaultTexture(resourceManager.GetTexture2D(ESpriteKey::Button1));
	
	button1.AddNeighbor(&button4, EWidgetNeighbor::Up);
	button1.AddNeighbor(&button2, EWidgetNeighbor::Down);
	
	button2.AddNeighbor(&button1, EWidgetNeighbor::Up);
	button2.AddNeighbor(&button3, EWidgetNeighbor::Down);
	
	button3.AddNeighbor(&button2, EWidgetNeighbor::Up);
	button3.AddNeighbor(&button4, EWidgetNeighbor::Down);
	
	button4.AddNeighbor(&button3, EWidgetNeighbor::Up);
	button4.AddNeighbor(&button1, EWidgetNeighbor::Down);
	
	button1.SetPosition(glm::vec2(40.0f, 50.0f));
	button2.SetPosition(glm::vec2(40.0f, 100.0f));
	button3.SetPosition(glm::vec2(40.0f, 150.0f));
	button4.SetPosition(glm::vec2(40.0f, 200.0f));
	
	constexpr auto buttonSize = glm::vec2(80.f, 40.0f);
	button1.SetSize(buttonSize);
	button2.SetSize(buttonSize);
	button3.SetSize(buttonSize);
	button4.SetSize(buttonSize);
	
	constexpr auto buttonScale = glm::vec2(0.5f);
	button1.SetScale(buttonScale);
	button2.SetScale(buttonScale);
	button3.SetScale(buttonScale);
	button4.SetScale(buttonScale);
	
	button1.BindLeftClick([](){ std::cout << "attack button clicked!\n"; });
	button2.BindLeftClick([](){ std::cout << "skill button clicked!\n"; });
	button3.BindLeftClick([](){ std::cout << "block button clicked!\n"; });
	button4.BindLeftClick([](){ std::cout << "move button clicked!\n"; });
	button2.BindSelected([](){ std::cout << "!!skill button selected!!\n"; });
	
	ui_panel_->AddWidget(button1);
	ui_panel_->AddWidget(button2);
	ui_panel_->AddWidget(button3);
	ui_panel_->AddWidget(button4);
}

void CombatManager::InitializeSkillButtons()
{
	// 1 2
	// 3 4
	// 5 6
	ResourceManager& resourceManager = ResourceManager::Instance();
	
	Button& button1 = skill_buttons_[0];	//NOLINT
	Button& button2 = skill_buttons_[1];	//NOLINT
	Button& button3 = skill_buttons_[2];	//NOLINT
	Button& button4 = skill_buttons_[3];	//NOLINT
	Button& button5 = skill_buttons_[4];	//NOLINT
	Button& button6 = skill_buttons_[5];	//NOLINT
	
	//button1.AddNeighbor(&button5, EWidgetNeighbor::Up);
	button1.AddNeighbor(&button3, EWidgetNeighbor::Down);
	//button1.AddNeighbor(&button2, EWidgetNeighbor::Left);
	button1.AddNeighbor(&button2, EWidgetNeighbor::Right);
	
	//button2.AddNeighbor(&button6, EWidgetNeighbor::Up);
	button2.AddNeighbor(&button4, EWidgetNeighbor::Down);
	button2.AddNeighbor(&button1, EWidgetNeighbor::Left);
	//button2.AddNeighbor(&button1, EWidgetNeighbor::Right);
	
	button3.AddNeighbor(&button1, EWidgetNeighbor::Up);
	button3.AddNeighbor(&button5, EWidgetNeighbor::Down);
	//button3.AddNeighbor(&button4, EWidgetNeighbor::Left);
	button3.AddNeighbor(&button4, EWidgetNeighbor::Right);
	
	button4.AddNeighbor(&button2, EWidgetNeighbor::Up);
	button4.AddNeighbor(&button6, EWidgetNeighbor::Down);
	button4.AddNeighbor(&button3, EWidgetNeighbor::Left);
	//button4.AddNeighbor(&button3, EWidgetNeighbor::Right);
	
	button5.AddNeighbor(&button3, EWidgetNeighbor::Up);
	//button5.AddNeighbor(&button1, EWidgetNeighbor::Down);
	//button5.AddNeighbor(&button6, EWidgetNeighbor::Left);
	button5.AddNeighbor(&button6, EWidgetNeighbor::Right);
	
	button6.AddNeighbor(&button4, EWidgetNeighbor::Up);
	//button6.AddNeighbor(&button2, EWidgetNeighbor::Down);
	button6.AddNeighbor(&button5, EWidgetNeighbor::Left);
	//button6.AddNeighbor(&button5, EWidgetNeighbor::Right);
	
	for (int i = 0; i < 6; ++i)
	{
		skill_buttons_[i].SetLabel("Skill " + std::to_string(i+1)); // NOLINT
		skill_buttons_[i].SetLabelBaseColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)); // NOLINT
		skill_buttons_[i].SetSize(glm::vec2(160.0f, 40.0f)); // NOLINT
		skill_buttons_[i].SetScale(glm::vec2(0.5f)); // NOLINT
		skill_buttons_[i].GetSpriteComponent()->SetDefaultTexture(resourceManager.GetTexture2D(ESpriteKey::Button1)); // NOLINT
		ui_panel_->AddWidget(skill_buttons_[i]); //NOLINT
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
	ResourceManager& resourceManager = ResourceManager::Instance();
	for (int i = 0; i < 4; ++i)
	{
		player_targeting_buttons_[i].SetLabel("Player Character " + std::to_string(i+1));	// NOLINT
		player_targeting_buttons_[i].SetLabelBaseColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));	// NOLINT
		player_targeting_buttons_[i].SetSize(glm::vec2(100.0f, 100.0f));	// NOLINT
		player_targeting_buttons_[i].SetScale(glm::vec2(0.5f));	// NOLINT
		player_targeting_buttons_[i].GetSpriteComponent()->SetDefaultTexture(resourceManager.GetTexture2D(ESpriteKey::Button1));	// NOLINT
		player_targeting_buttons_[i].SetPosition(glm::vec2(0.0f));	// NOLINT
		player_targeting_buttons_[i].BindLeftClick([](){std::cout << "Player targeting button pressed!\n";});	// NOLINT
		ui_panel_->AddWidget(player_targeting_buttons_[i]);	// NOLINT
		player_targeting_buttons_[i].SetVisible(false);	// NOLINT
		player_targeting_buttons_[i].SetActive(false);	// NOLINT
		
	}
	
	// Neighboring and position will be updated dynamically
}

void CombatManager::InitializeEnemyTargetingButtons()
{
	ResourceManager& resourceManager = ResourceManager::Instance();
	for (int i = 0; i < 7; ++i)
	{
		enemy_targeting_buttons_[i].SetLabel("Player Character " + std::to_string(i+1));	//NOLINT
		enemy_targeting_buttons_[i].SetLabelBaseColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));	//NOLINT
		enemy_targeting_buttons_[i].SetSize(glm::vec2(100.0f, 100.0f));	//NOLINT
		enemy_targeting_buttons_[i].SetScale(glm::vec2(0.5f));	//NOLINT
		enemy_targeting_buttons_[i].GetSpriteComponent()->SetDefaultTexture(resourceManager.GetTexture2D(ESpriteKey::Button1));	//NOLINT
		enemy_targeting_buttons_[i].SetPosition(glm::vec2(0.0f));	//NOLINT
		enemy_targeting_buttons_[i].BindLeftClick([](){std::cout << "Enemy targeting button pressed!\n";});	//NOLINT
		ui_panel_->AddWidget(enemy_targeting_buttons_[i]);	//NOLINT
		enemy_targeting_buttons_[i].SetVisible(false);	//NOLINT
		enemy_targeting_buttons_[i].SetActive(false);	//NOLINT
	}
	
	// Neighboring and position will be updated dynamically
}

void CombatManager::InitializeZoneTargetingButtons()
{
	// 4 Enemy back
	// 3 Enemy front
	// 2 Player front
	// 1 Player back
	
	Button& zone1 = zone_targeting_buttons_[0]; // NOLINT
	Button& zone2 = zone_targeting_buttons_[1]; // NOLINT
	Button& zone3 = zone_targeting_buttons_[2]; // NOLINT
	Button& zone4 = zone_targeting_buttons_[3]; // NOLINT
	
	ResourceManager& resourceManager = ResourceManager::Instance();
	for (int i = 0; i < 4; ++i)
	{
		zone_targeting_buttons_[i].SetLabel("Player Character " + std::to_string(i+1));	// NOLINT
		zone_targeting_buttons_[i].SetLabelBaseColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));	// NOLINT
		zone_targeting_buttons_[i].SetSize(glm::vec2(100.0f, 100.0f));	// NOLINT
		zone_targeting_buttons_[i].SetScale(glm::vec2(0.5f));	// NOLINT
		zone_targeting_buttons_[i].GetSpriteComponent()->SetDefaultTexture(resourceManager.GetTexture2D(ESpriteKey::Button1));	// NOLINT
		zone_targeting_buttons_[i].SetPosition(glm::vec2(0.0f));	// NOLINT
		zone_targeting_buttons_[i].BindLeftClick([](){std::cout << "Zone targeting button pressed!\n";});	// NOLINT
		ui_panel_->AddWidget(zone_targeting_buttons_[i]);	// NOLINT
		zone_targeting_buttons_[i].SetVisible(false);	// NOLINT
		zone_targeting_buttons_[i].SetActive(false);	// NOLINT
	}
	
	
	zone1.AddNeighbor(&zone2, EWidgetNeighbor::Up);
	zone2.AddNeighbor(&zone3, EWidgetNeighbor::Up);
	zone2.AddNeighbor(&zone1, EWidgetNeighbor::Down);
	zone3.AddNeighbor(&zone4, EWidgetNeighbor::Up);
	zone3.AddNeighbor(&zone2, EWidgetNeighbor::Down);
	zone4.AddNeighbor(&zone3, EWidgetNeighbor::Down);
}
