#ifndef AUDIO_MANAGER_HPP
#define AUDIO_MANAGER_HPP
#pragma once

#include "miniaudio/miniaudio.h"
#include "robin_hood_hash/robin_hood.h"

#include "EngineDataStructures.hpp"

#include <vector>
#include <string>

class AudioManager
{
    ma_engine* audio_engine_;
    std::vector<ma_sound*> sounds_;
    robin_hood::unordered_map<ESoundType, ma_sound_group*> sound_groups_;
    
    AudioManager();
    ~AudioManager();
    
public:
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
    
    static AudioManager& Instance();
    
    void Initialize();
    
    int LoadSound(const std::string& filename, const ESoundType soundType);
    void PlaySound(const unsigned int index, const bool rewind = true) const;
    void StopSound(const unsigned int index, const bool rewind = true) const;
    
    void ApplyVolumeChange();
};
#endif