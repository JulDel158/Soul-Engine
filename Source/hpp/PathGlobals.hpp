#ifndef PATH_GLOBALS_H
#define PATH_GLOBALS_H

#include <string_view>

// IMPORTANT: Remember to add the null terminator to your strings: ---> \0 <---

// vertex shaders
constexpr std::string_view VERTEX_SHADER1 = "Assets/Shaders/BasicSprite.vert\0";

// fragment shaders
constexpr std::string_view FRAGMENT_SHADER1 = "Assets/Shaders/BasicSprite.frag\0";

// textures
constexpr std::string_view TEXTURE1 = "D:/Projects/Soul/Assets/Textures/awesomeface.png\0";

// Settings/Configs
constexpr std::string_view GRAPHIC_SETTINGS = "Assets/Settings/Graphics.json\0";
constexpr std::string_view SCREEN_WIDTH = "screenWidth\0";
constexpr std::string_view SCREEN_HEIGHT = "screenHeight\0";
constexpr std::string_view VSYNC = "vsync\0";

#endif