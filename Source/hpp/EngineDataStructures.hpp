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

#endif