#ifndef STRING_GLOBALS_HPP
#define STRING_GLOBALS_HPP
#pragma once

#include <string_view>

// IMPORTANT: Remember to add the null terminator to your strings: ---> \0 <---

// vertex shader paths
constexpr std::string_view V_SPRITE_SHADER_BASE = "Assets/Shaders/BasicSprite.vert\0";
constexpr std::string_view V_TEXT_SHADER_BASE = "Assets/Shaders/BasicText.vert\0";

// fragment shader paths
constexpr std::string_view F_SPRITE_SHADER_BASE = "Assets/Shaders/BasicSprite.frag\0";
constexpr std::string_view F_TEXT_SHADER_BASE = "Assets/Shaders/BasicText.frag\0";

// geometry shader paths

// textures paths
constexpr std::string_view PLACEHOLDER_TEXTURE = "Assets/Textures/place_holder.png\0";
constexpr std::string_view MISSING_TEXTURE = "Assets/Textures/missing_texture.png\0";
constexpr std::string_view TEXTURE1 = "Assets/Textures/awesomeface.png\0";
constexpr std::string_view TEST_ANIM_FRAME_1 = "Assets/Textures/test_anim_1.png\0";
constexpr std::string_view TEST_ANIM_FRAME_2 = "Assets/Textures/test_anim_2.png\0";
constexpr std::string_view TEST_ANIM_FRAME_3 = "Assets/Textures/test_anim_3.png\0";

// font paths
constexpr std::string_view FONT_ARIAL_PATH = "Assets/Fonts/arial.ttf\0";

// audio paths
constexpr std::string_view AUDIO_1 = "Assets/Audio/Fragmentum.wav\0";

// shader keys
constexpr std::string_view SPRITE_RENDERER_PROGRAM1 = "SPRITE\0";
constexpr std::string_view FONT_RENDERER_PROGRAM1 = "FONT\0";

// font keys
constexpr std::string_view FONT_ARIAL = "arial\0";

// uniforms
constexpr std::string_view IMAGE_UNIFORM = "image\0";
constexpr std::string_view PROJECTION_UNIFORM = "projection\0";
constexpr std::string_view MODEL_UNIFORM = "model\0";
constexpr std::string_view SPRITE_COLOR_UNIFORM = "spriteColor\0";
constexpr std::string_view TEXT_COLOR_UNIFORM = "textColor\0";
constexpr std::string_view TEXT_UNIFORM = "text\0";

// Settings/Configs
constexpr std::string_view SETTINGS_PATH = "Assets/Settings/Settings.json\0";
constexpr std::string_view SCREEN_WIDTH = "screenWidth\0";
constexpr std::string_view SCREEN_HEIGHT = "screenHeight\0";
constexpr std::string_view GENERAL_VOLUME = "generalVolume\0";
constexpr std::string_view MUSIC_VOLUME = "musicVolume\0";
constexpr std::string_view EFFECTS_VOLUME = "effectsVolume\0";
constexpr std::string_view DIALOGUE_VOLUME = "dialogueVolume\0";
constexpr std::string_view VSYNC = "vsync\0";

// Log
constexpr std::string_view LOG_PATH = "Assets/Log/Soul.log\0";

#endif