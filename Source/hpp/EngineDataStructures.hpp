#ifndef ENGINE_DATA_STRUCTURES_HPP
#define ENGINE_DATA_STRUCTURES_HPP
#pragma once

#include "glm/vec2.hpp"

#include <string>

struct Settings
{
public:
    int screen_width_ = 0;
    int screen_height_ = 0;
    float general_volume_ = 0.f;
    float music_volume_ = 0.f;
    float effects_volume_ = 0.f;
    float dialogue_volume_ = 0.f;
    bool vsync_ = false;
};

enum class ESoundType : unsigned char
{
    General = 0,
    Music = 1,
    Effect = 2,
    Dialogue = 3,
};

enum class ESpriteCentering : unsigned char
{
    Center =        0,
    Top_Left =      1,
    Bottom_Left =   2,
    Top_Right =     3,
    Bottom_Right =  4,
};

enum class EGameState : unsigned int
{
    None =              0,
    MainMenu = 			1,
    Settings,
	Credits,
    
    InGame_Running,
    InGame_Paused,
	InGame_Settings,
	
    Loading,
    Unloading,
	
	Closing
};

enum class ESpriteKey : unsigned int
{
	None = 0,
	
	Missing,
	PlaceHolder,
	
	Debug1,
	Debug2,
	Debug3,
	Debug4,
	Debug5,
	Debug6,
	Debug7,
	Debug8,
	Debug9,
	
	Player_Idle_1,
	Player_Idle_2,
	Player_Idle_3,
	
	Player_Left_1,
	Player_Left_2,
	Player_Left_3,
	
	Player_Right_1,
	Player_Right_2,
	Player_Right_3,
	
	Player_Up_1,
	Player_Up_2,
	Player_Up_3,
	
	Player_Down_1,
	Player_Down_2,
	Player_Down_3,
};

struct TextCharacter
{
    unsigned int id_; // ID handle of the glyph texture
    glm::ivec2 size_; // size of glyph
    glm::ivec2 bearing_; // offset from baseline to left/top of glyph
    unsigned int advance_; // horizontal offset to advance to next glyph
    
    inline TextCharacter(const unsigned int id, const glm::ivec2 size, const glm::ivec2 bearing, const unsigned int advance) : 
    id_(id), 
    size_(size),
    bearing_(bearing),
    advance_(advance)
    {}
    
    inline TextCharacter() : 
    id_(0), 
    size_(0, 0), 
    bearing_(0), 
    advance_(0)
    {}
};

enum class EInputActionType : unsigned char
{
    None = 0,
    Keyboard = 1,
    Mouse_Cursor = 2,
    Mouse_Button = 3,
    Mouse_Wheel = 4,
    Gamepad_Button = 5,
    Gamepad_Axes = 6
};

enum class ECursorDataMode : unsigned char
{
    Position =          0, // value is the cursor position as provided by the callback.
    Direction =         1, // Value will be either -1.0, 0.0, or 1.0 for each axis.
    Normalized =        2, // value will be a normalized vector of the difference of the cursor position between previous and current frames.
    Additive =          3, // value will be the current stored value + the difference of the previous and current frames, value is uncapped.
    Additive_Clamped =  4  // value is the same as additive, but it gets clamped between 0 and InputAction.clamp_max_.
};

enum class ELogLevel : unsigned char
{
    Debug = 0,
    Info = 1,
    Warning = 2,
    Error = 3,
};

enum class EWidgetNeighbor : unsigned char
{
	Up = 0,
	Down = 1,
	Left = 2,
	Right = 3,
	Count = 4
};

enum class EPanelType : unsigned int
{
	// Main Menu
	MainMenu = 0,
	Settings_Main = 1,
	Credits,
	
	//  Pause
	Pause,
	Settings_InGame,
	
	// In game
	Game_1,
	Game_2,
	Game_3,
	Game_4,
	Game_5,
	Game_6,
	Game_7,
	Game_8,
	Game_9,
	Game_10,
	
	// Loading
	Loading_1,
	Loading_2,
	Loading_3,
	Loading_4,
	Loading_5,
	Loading_6,
	Loading_7,
	Loading_8,
	Loading_9,
	
	Count
};

enum class EComponentClassType : int
{
	Base = 0,
	SpriteComponent,
	MovementComponent,
	BoxCollisionComponent,
	HealthComponent
	//TODO: Add any new component type here
};

enum class EGameObjectClassType : int
{
	Base = 0,
	BackgroundTile,
	Character,
	PlayerCharacter,
	CombatCharacter
	// TODO: Add any new gameobject type here
};

namespace  DataConverter
{
	inline std::string ToString(const EGameObjectClassType classType)
	{
		std::string result;
		
		switch (classType)
		{
		case EGameObjectClassType::Base:
			result = "Base";
			break;
		case EGameObjectClassType::BackgroundTile:
			result = "BackgroundTile";
			break;
		case EGameObjectClassType::Character:
			result = "Character";
			break;
		case EGameObjectClassType::PlayerCharacter:
			result = "PlayerCharacter";
			break;
		case EGameObjectClassType::CombatCharacter:
			result = "CombatCharacter";
		}
		
		return result;
	}
	inline std::string ToString(const EComponentClassType componentType)
	{
		std::string result;
		
		switch (componentType)
		{
		case EComponentClassType::Base:
			result = "Base";
			break;
		case EComponentClassType::SpriteComponent:
			result = "SpriteComponent";
			break;
		case EComponentClassType::MovementComponent:
			result = "MovementComponent";
			break;
		case EComponentClassType::BoxCollisionComponent:
			result = "BoxCollisionComponent";
			break;
		case EComponentClassType::HealthComponent:
			result = "HealthComponent";
			break;
		}
		
		
		return result;
	}
	
	inline std::string ToString(const ESpriteKey spriteId)
	{
		std::string result;
		switch (spriteId)
		{
		case ESpriteKey::None:
			result = "None";
			break;
		case ESpriteKey::Missing:
			result = "Missing";
			break;
		case ESpriteKey::PlaceHolder:
			result = "PlaceHolder";
			break;
		case ESpriteKey::Debug1:
			result = "Debug1";
			break;
		case ESpriteKey::Debug2:
			result = "Debug2";
			break;
		case ESpriteKey::Debug3:
			result = "Debug3";
			break;
		case ESpriteKey::Debug4:
			result = "Debug4";
			break;
		case ESpriteKey::Debug5:
			result = "Debug5";
			break;
		case ESpriteKey::Debug6:
			result = "Debug6";
			break;
		case ESpriteKey::Debug7:
			result = "Debug7";
			break;
		case ESpriteKey::Debug8:
			result = "Debug8";
			break;
		case ESpriteKey::Debug9:
			result = "Debug9";
			break;
		case ESpriteKey::Player_Idle_1:
			result = "Player_Idle_1";
			break;
		case ESpriteKey::Player_Idle_2:
			result = "Player_Idle_2";
			break;
		case ESpriteKey::Player_Idle_3:
			result = "Player_Idle_3";
			break;
		case ESpriteKey::Player_Left_1:
			result = "Player_Left_1";
			break;
		case ESpriteKey::Player_Left_2:
			result = "Player_Left_2";
			break;
		case ESpriteKey::Player_Left_3:
			result = "Player_Left_3";
			break;
		case ESpriteKey::Player_Right_1:
			result = "Player_Right_1";
			break;
		case ESpriteKey::Player_Right_2:
			result = "Player_Right_2";
			break;
		case ESpriteKey::Player_Right_3:
			result = "Player_Right_3";
			break;
		case ESpriteKey::Player_Up_1:
			result = "Player_Up_1";
			break;
		case ESpriteKey::Player_Up_2:
			result = "Player_Up_2";
			break;
		case ESpriteKey::Player_Up_3:
			result = "Player_Up_3";
			break;
		case ESpriteKey::Player_Down_1:
			result = "Player_Down_1";
			break;
		case ESpriteKey::Player_Down_2:
			result = "Player_Down_2";
			break;
		case ESpriteKey::Player_Down_3:
			result = "Player_Down_3";
			break;
		default:
			result = "Unknown/Missing";
			break;
		}
		
		return result;
	}
}

#endif