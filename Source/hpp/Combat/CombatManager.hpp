#ifndef COMBAT_MANAGER_HPP
#define COMBAT_MANAGER_HPP
#pragma once

#include "UI/Panel.hpp"
#include "UI/Button.hpp"
#include "GameDataStructures.hpp"

#include <vector>

class CombatCharacter;
class Zone;

class CombatManager
{
private:
	CombatCharacter* player_characters_[4];
	std::vector<CombatCharacter*> enemy_characters_;
	Zone* zones_[4];
	
	// UI
	Panel* ui_panel_;
	Button* ability_buttons_;
	Button* skill_buttons_;
	// For the targeting buttons, we will have to dynamically update neighbors as we move characters around the field
	Button* player_targeting_buttons_; 
	Button* enemy_targeting_buttons_;
	Button* zone_targeting_buttons_;
	Button* end_turn_button_;
	
	float row_positions_[4];
	float column_positions_[6];
	bool occupied_positions_[4][6];
	
	unsigned int selected_character_index_;
	bool is_player_turn_;
	
	ECombatState combat_state_;
	
	CombatManager();
	~CombatManager();
	
public:
	CombatManager(const CombatManager&) = delete;
	CombatManager& operator=(const CombatManager&) = delete;
	
	static CombatManager& Instance();
	Panel* GetUIPanel() { return ui_panel_; } // NOLINT
	
	void Initialize();
	void BeginCombat(std::vector<CombatCharacter*> enemies);
	void EndCombat();
	
	void SetPlayerCharacter(CombatCharacter* characters, const unsigned int index) { if (index < 4) { player_characters_[index] = characters; } }
	
	bool IsPositionOccupied(ECombatPosition targetRow, unsigned int column) const;
	bool MoveCharacter(ECombatPosition currentPosition, unsigned int column);
	glm::vec2 GetLocationPosition(ECombatPosition row, unsigned int column) const;
	
	inline bool IsPlayerTurn() const { return is_player_turn_; }
	inline bool IsEnemyTurn() const { return !is_player_turn_; }
	
private:
	void InitializeAbilityButtons() const;
	void InitializeSkillButtons() const;
	void InitializePlayerTargetingButtons() const;
	void InitializeEnemyTargetingButtons() const;
	void InitializeZoneTargetingButtons() const;
	void InitializeEndTurnButton() const;
};

#endif