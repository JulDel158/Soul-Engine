#ifndef ENGINE_DATA_STRUCTURES_H
#define ENGINE_DATA_STRUCTURES_H

#include "glm/vec2.hpp"

struct Settings
{
public:
    int screen_width_;
    int screen_height_;
    bool vsync_;
};

enum class ESpriteCentering : unsigned char
{
    Center = 0,
    Top_Left = 1,
    Bottom_Left = 2,
    Top_Right = 3,
    Bottom_Right = 4,
};

enum class EGameState : unsigned char
{
    None = 0,
    MainMenu = 1,
    Settings = 2,
    
    InGame_Running = 3,
    InGame_Paused = 4,
    InGame_Loading = 5,
    
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

union UInputData
{
    glm::vec2 vec2;
    float alpha;
    bool flag;
};

#endif