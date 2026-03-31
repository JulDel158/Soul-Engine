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
	Panel* ui_panel_;
	Button* ability_buttons_;
	Button* skill_buttons_;
	// For the targeting buttons, we will have to dynamically update neighbors as we move characters around the field
	Button* player_targeting_buttons_; 
	Button* enemy_targeting_buttons_;
	Button* zone_targeting_buttons_;
	
	CombatManager();
	~CombatManager();
	
public:
	CombatManager(const CombatManager&) = delete;
	CombatManager& operator=(const CombatManager&) = delete;
	
	static CombatManager& Instance();
	
	Panel* GetUIPanel() { return ui_panel_; } // NOLINT
	
	void Initialize();
	
	void BeginCombat();
	void EndCombat();
	
private:
	void InitializeAbilityButtons();
	void InitializeSkillButtons();
	void InitializePlayerTargetingButtons();
	void InitializeEnemyTargetingButtons();
	void InitializeZoneTargetingButtons();
};

#endif