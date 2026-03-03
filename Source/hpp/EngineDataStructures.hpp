#ifndef ENGINE_DATA_STRUCTURES_H
#define ENGINE_DATA_STRUCTURES_H

#include "glm/vec2.hpp"

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

enum class EGameState : unsigned char
{
    None =              0,
    MainMenu =          1,
    Settings =          2,
    
    InGame_Running =    3,
    InGame_Paused =     4,
    InGame_Loading =    5,
    
    Exiting = 6
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

#endif