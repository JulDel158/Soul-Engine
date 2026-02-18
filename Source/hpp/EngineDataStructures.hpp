#ifndef ENGINE_DATA_STRUCTURES_H
#define ENGINE_DATA_STRUCTURES_H

struct Settings
{
public:
    int screen_width_;
    int screen_height_;
};

enum class ESpriteCentering : unsigned char
{
    CENTER = 0,
    TOP_LEFT = 1,
    BOTTOM_LEFT = 2,
    TOP_RIGHT = 3,
    BOTTOM_RIGHT = 4,
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