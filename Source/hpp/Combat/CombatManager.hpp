#ifndef COMBAT_MANAGER_HPP
#define COMBAT_MANAGER_HPP
#pragma once

#include "UI/Panel.hpp"
#include "UI/Button.hpp"

#include <vector>

class CombatCharacter;
class Zone;

class CombatManager
{
private:
	std::vector<CombatCharacter*> player_characters_;
	std::vector<CombatCharacter*> enemy_characters_;
	Zone* zones_[4];
	bool is_player_turn_;
	unsigned int selected_character_index_;
	
	// UI
	Panel ui_panel_;
	Button ability_buttons_[4];
	Button skill_buttons_[6];
	// For the targeting buttons, we will have to dynamically update neighbors as we move characters around the field
	Button player_targeting_buttons_[4]; 
	Button enemy_targeting_buttons_[7];
	Button zone_targeting_buttons_[4];
	
	CombatManager();
	~CombatManager();
	
public:
	CombatManager(const CombatManager&) = delete;
	CombatManager& operator=(const CombatManager&) = delete;
	
	static CombatManager& Instance();
	
	Panel* GetUIPanel() { return &ui_panel_; } // NOLINT
	
	void Initialize();
	
private:
	void InitializeAbilityButtons();
	void InitializeSkillButtons();
	void InitializePlayerTargetingButtons();
	void InitializeEnemyTargetingButtons();
	void InitializeZoneTargetingButtons();
};

#endif