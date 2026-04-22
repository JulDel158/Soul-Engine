#include "Combat/CombatManager.hpp"

#include "EngineDataStructures.hpp"
#include "Components/SpriteComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Utils/ResourceManager.hpp"
#include "Characters/CombatCharacter.hpp"
#include "Combat/Zones/Zone.hpp"

#include <iostream>
#include <utility>


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
player_characters_{nullptr},
zones_{nullptr}, 
occupied_positions_{{false}},
selected_character_index_(0),
is_player_turn_(false),
combat_state_(ECombatState::None)
{
	enemy_characters_.reserve(6);

	ui_panel_ = new Panel();
	ability_buttons_ = new Button[ABILITY_BUTTON_COUNT];
	skill_buttons_ = new Button[SKILL_BUTTON_COUNT];
	for (auto& button : player_targeting_buttons_)
	{
		button = new Button();
	}
	for (auto& button : enemy_targeting_buttons_)
	{
		button = new Button();
	}
	zone_targeting_buttons_ = new Button[ZONE_TARGETS_BUTTON_COUNT];
	end_turn_button_ = new Button();

	row_positions_[0] = 0.2f;
	row_positions_[1] = 0.4f;
	row_positions_[2] = 0.6f;
	row_positions_[3] = 0.8f;

	column_positions_[0] = 1.0f / 8.0f;
	column_positions_[1] = 2.0f / 8.0f;
	column_positions_[2] = 3.0f / 8.0f;
	column_positions_[3] = 4.0f / 8.0f;
	column_positions_[4] = 5.0f / 8.0f;
	column_positions_[5] = 6.0f / 8.0f;
}

CombatManager::~CombatManager()
{
	delete ui_panel_;
	delete[] ability_buttons_;
	delete[] skill_buttons_;
	for (auto& button : player_targeting_buttons_)
	{
		delete button;
	}
	for (auto& button : enemy_targeting_buttons_)
	{
		delete button;
	}
	delete[] zone_targeting_buttons_;
	delete end_turn_button_;
}

CombatManager& CombatManager::Instance()
{
	static auto instance = CombatManager();
	return instance;
}

void CombatManager::Initialize()
{
	InitializeAbilityButtons();
	InitializeSkillButtons();
	InitializePlayerTargetingButtons();
	InitializeEnemyTargetingButtons();
	InitializeZoneTargetingButtons();
	InitializeEndTurnButton();
	
	ui_panel_->SetActive(false);
	ui_panel_->SetVisible(false);
}

void CombatManager::BeginCombat(std::vector<CombatCharacter*> enemies)
{
	enemy_characters_.clear();
	enemy_characters_ = std::move(enemies);
	
	is_player_turn_ = true;
	
	ui_panel_->SetActive(true);
	ui_panel_->SetVisible(true);
	
	for (unsigned int i = 0; i < 4; ++i)
	{
		if (player_characters_[i] == nullptr)
		{
			continue;
		}
		player_characters_[i]->SetPosition(GetLocationPosition(player_characters_[i]->GetCombatPosition(), i));
	}
	
	for (unsigned int i = 0; i < enemy_characters_.size(); ++i)
	{
		if (enemy_characters_[i] == nullptr)
		{
			continue;
		}
		enemy_characters_[i]->SetPosition(GetLocationPosition(enemy_characters_[i]->GetCombatPosition(), i));
	}
}

void CombatManager::EndCombat()
{
}


bool CombatManager::IsPositionOccupied(const ECombatPosition targetRow, const unsigned int column) const
{
	if (targetRow == ECombatPosition::None || column >= 6)
	{
		// Invalid row and column
		return false;
	}
	
	return occupied_positions_[static_cast<unsigned int>(targetRow)][column];
}

bool CombatManager::MoveCharacter(const ECombatPosition currentPosition, const unsigned int column)
{
	switch (currentPosition)
	{
	case ECombatPosition::None:
		return false;
	case ECombatPosition::BackEnemy:
	case ECombatPosition::FrontEnemy:
		if (column >= 6 || enemy_characters_[column] == nullptr)
		{
			return false;
		}
		enemy_characters_[column]->Move(); // This should probably be called later but for now will do
		enemy_characters_[column]->SetPosition(GetLocationPosition(enemy_characters_[column]->GetCombatPosition(), column));
		break;
	case ECombatPosition::FrontPlayer:
	case ECombatPosition::BackPlayer:
		if (column > 3 || player_characters_[column] == nullptr)
		{
			return false;
		}
		player_characters_[column]->Move(); // This should probably be called later 
		player_characters_[column]->SetPosition(GetLocationPosition(player_characters_[column]->GetCombatPosition(), column));
		break;
	}
	
	occupied_positions_[static_cast<unsigned int>(currentPosition)][column] = false;
	occupied_positions_[static_cast<unsigned int>(GetMoveDestination(currentPosition))][column] = true;
	
	return true;
}

glm::vec2 CombatManager::GetLocationPosition(ECombatPosition row, unsigned int column) const
{
	if (row == ECombatPosition::BackPlayer || row == ECombatPosition::FrontPlayer)
	{
		column += 1;
	}
	
	if (row == ECombatPosition::None || column >= 6)
	{
		return glm::vec2(0.0f);
	}
	
	const auto& resourceManager = ResourceManager::Instance();
	const auto settings = resourceManager.GetSettings();
	glm::vec2 result;
	
	result.x = row_positions_[static_cast<unsigned int>(row)] * static_cast<float>(settings.screen_height_);
	result.y = column_positions_[column] * static_cast<float>(settings.screen_width_);
	
	return result;
}

void CombatManager::InitializeAbilityButtons() const
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

void CombatManager::InitializeSkillButtons() const
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
	
	for (unsigned int i = 0; i < SKILL_BUTTON_COUNT; ++i)
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

void CombatManager::InitializePlayerTargetingButtons() const
{
	ResourceManager& resourceManager = ResourceManager::Instance();
	for (unsigned int i = 0; i < PLAYER_TARGETS_BUTTON_COUNT; ++i)
	{
		player_targeting_buttons_[i]->SetLabel("Player Character " + std::to_string(i+1));	// NOLINT
		player_targeting_buttons_[i]->SetLabelBaseColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));	// NOLINT
		player_targeting_buttons_[i]->SetSize(glm::vec2(100.0f, 100.0f));	// NOLINT
		player_targeting_buttons_[i]->SetScale(glm::vec2(0.5f));	// NOLINT
		player_targeting_buttons_[i]->GetSpriteComponent()->SetDefaultTexture(resourceManager.GetTexture2D(ESpriteKey::Button1));	// NOLINT
		player_targeting_buttons_[i]->SetPosition(glm::vec2(0.0f));	// NOLINT
		player_targeting_buttons_[i]->BindLeftClick([](){std::cout << "Player targeting button pressed!\n";});	// NOLINT
		ui_panel_->AddWidget(*player_targeting_buttons_[i]);
		player_targeting_buttons_[i]->SetVisible(false);	
		player_targeting_buttons_[i]->SetActive(false);	
		
	}
	
	// Neighboring and position will be updated dynamically
}

void CombatManager::InitializeEnemyTargetingButtons() const
{
	ResourceManager& resourceManager = ResourceManager::Instance();
	for (unsigned int i = 0; i < ENEMY_TARGETS_BUTTON_COUNT; ++i)
	{
		enemy_targeting_buttons_[i]->SetLabel("Player Character " + std::to_string(i+1));	//NOLINT
		enemy_targeting_buttons_[i]->SetLabelBaseColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));	//NOLINT
		enemy_targeting_buttons_[i]->SetSize(glm::vec2(100.0f, 100.0f));	//NOLINT
		enemy_targeting_buttons_[i]->SetScale(glm::vec2(0.5f));	//NOLINT
		enemy_targeting_buttons_[i]->GetSpriteComponent()->SetDefaultTexture(resourceManager.GetTexture2D(ESpriteKey::Button1));	//NOLINT
		enemy_targeting_buttons_[i]->SetPosition(glm::vec2(0.0f));	//NOLINT
		enemy_targeting_buttons_[i]->BindLeftClick([](){std::cout << "Enemy targeting button pressed!\n";});	//NOLINT
		ui_panel_->AddWidget(*enemy_targeting_buttons_[i]);	//NOLINT
		enemy_targeting_buttons_[i]->SetActive(false);	//NOLINT
	}
	
	// Neighboring and position will be updated dynamically
}

void CombatManager::InitializeZoneTargetingButtons() const
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
	for (unsigned int i = 0; i < ZONE_TARGETS_BUTTON_COUNT; ++i)
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

void CombatManager::InitializeEndTurnButton() const
{
	end_turn_button_->SetLabel("End Turn");	// NOLINT
	end_turn_button_->SetLabelBaseColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));	// NOLINT
	end_turn_button_->SetSize(glm::vec2(100.0f, 100.0f));	// NOLINT
	end_turn_button_->SetScale(glm::vec2(0.5f));	// NOLINT
	end_turn_button_->GetSpriteComponent()->SetDefaultTexture( ResourceManager::Instance().GetTexture2D(ESpriteKey::Button1));
	end_turn_button_->SetPosition(glm::vec2(0.0f));	// NOLINT
	end_turn_button_->BindLeftClick([](){std::cout << "Zone targeting button pressed!\n";});
}

// This function is in charge of linking target buttons and delinking them based on mode
void CombatManager::UpdateTargetingButtons(const ETargetingType targetingType, const ECombatPosition targetZone, const bool isPlayerCharacter, const EPartyType targetParty, const CombatCharacter * const caster)
{
	// DEPRECATED
	// for (auto& button : enemy_targeting_buttons_)
	// {
	// 	button->ClearNeighbors(); // NOLINT
	// }
	//
	// for (auto& button : player_targeting_buttons_)
	// {
	// 	button->ClearNeighbors(); // NOLINT
	// }
	
	active_targeting_buttons_.clear();
	active_targeting_buttons_.reserve(MAX_PLAYER_COUNT + enemy_characters_.size());
	
	for (const auto& playerCharacter : player_characters_)
	{
		if (playerCharacter == nullptr || playerCharacter->GetHealthComponent()->IsDead())
		{
			continue;
		}
		
		if (playerCharacter->IsValidTarget(targetingType, targetZone, isPlayerCharacter, caster == playerCharacter, targetParty))
		{
			active_targeting_buttons_.emplace_back(playerCharacter->GetTargetingButton());
		}
	}
	
	for (const auto& enemy : enemy_characters_)
	{
		if (enemy == nullptr || enemy->GetHealthComponent()->IsDead())
		{
			continue;
		}
		
		if (enemy->IsValidTarget(targetingType, targetZone, !isPlayerCharacter, caster == enemy, targetParty))
		{
			active_targeting_buttons_.emplace_back(enemy->GetTargetingButton());
		}
	}
	
	for (const auto& zone : zones_)
	{
		if (zone == nullptr)
		{
			continue;
		}
		
		if (zone->IsValidTarget(targetingType, targetZone))
		{
			active_targeting_buttons_.emplace_back(zone->GetTargetingButton());
		}
	}
	
	for (int i = 0; i < static_cast<int>(active_targeting_buttons_.size()) - 1; ++i)
	{
		active_targeting_buttons_[i]->ClearNeighbors();
		active_targeting_buttons_[i]->SetActive(true);
		active_targeting_buttons_[i]->SetVisible(true);
		active_targeting_buttons_[i]->AddNeighbor(active_targeting_buttons_[i+1], EWidgetNeighbor::Right);
		active_targeting_buttons_[i+1]->AddNeighbor(active_targeting_buttons_[i], EWidgetNeighbor::Left);
	}
}
