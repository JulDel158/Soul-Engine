#ifndef ENGINE_DATA_STRUCTURES_H
#define ENGINE_DATA_STRUCTURES_H

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

#endif