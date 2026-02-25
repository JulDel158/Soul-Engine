#include "Audio/AudioManager.hpp"

#include <iostream>
#include <ranges>

namespace
{
    constexpr unsigned int SOUNDS_ALLOCATION = 100;
}

AudioManager::AudioManager() :
audio_engine_(new ma_engine)
{
    sounds_.reserve(SOUNDS_ALLOCATION);
    
    sound_groups_[ESoundType::General] = new ma_sound_group();
    sound_groups_[ESoundType::Music] = new ma_sound_group();
    sound_groups_[ESoundType::Dialogue] = new ma_sound_group();
    sound_groups_[ESoundType::Effect] = new ma_sound_group();
    
    Initialize();
}

AudioManager::~AudioManager()
{
    ma_engine_uninit(audio_engine_);
    delete audio_engine_;
    
    for (auto& sound : sounds_)
    {
        ma_sound_uninit(sound);
        delete sound;
    }
    sounds_.clear();
    
    for (auto& soundGroup : sound_groups_ | std::views::values)
    {
        ma_sound_group_uninit(soundGroup);
        delete soundGroup;
        soundGroup = nullptr;
    }
    sound_groups_.clear();
}

AudioManager& AudioManager::Instance()
{
    static auto instance = AudioManager();
    return instance;
}

void AudioManager::Initialize()
{
    if (const ma_result result = ma_engine_init(nullptr, audio_engine_); result != MA_SUCCESS) {
        std::cout << "ERROR::AudioManager: Failed to initialize the engine: " << result << std::endl;
        return;
    }
    
    if (ma_sound_group_init(audio_engine_, 0 ,nullptr ,sound_groups_[ESoundType::General]) != MA_SUCCESS)
    {
        std::cout << "ERROR::AudioManager: Failed to initialize the sound group: ESoundType::General" << std::endl;
    }
    
    if (ma_sound_group_init(audio_engine_, 0 ,sound_groups_[ESoundType::General] ,sound_groups_[ESoundType::Music]) != MA_SUCCESS)
    {
        std::cout << "ERROR::AudioManager: Failed to initialize the sound group: ESoundType::Music" << std::endl;
    }
    
    if (ma_sound_group_init(audio_engine_, 0 ,sound_groups_[ESoundType::General] ,sound_groups_[ESoundType::Effect]) != MA_SUCCESS)
    {
        std::cout << "ERROR::AudioManager: Failed to initialize the sound group: ESoundType::Effect" << std::endl;
    }
    
    if (ma_sound_group_init(audio_engine_, 0 ,sound_groups_[ESoundType::General] ,sound_groups_[ESoundType::Dialogue]) != MA_SUCCESS)
    {
        std::cout << "ERROR::AudioManager: Failed to initialize the sound group: ESoundType::Dialogue" << std::endl;
    }
    
    ApplyVolumeChange();
}

int AudioManager::LoadSound(const std::string& filename, const ESoundType soundType)
{
    if (audio_engine_ == nullptr)
    {
        return -1;
    }
    
    const auto sound = new ma_sound();
    
    if (ma_sound_init_from_file(audio_engine_, filename.c_str(), 
        0, sound_groups_[soundType], nullptr, sound) != MA_SUCCESS) 
    {
        return -1;
    }

    sounds_.push_back(sound);
    
    // we return an int because if we fail to load a sound, we do not want to provide an invalid index
    return static_cast<int>(sounds_.size() - 1);
}

void AudioManager::PlaySound(const unsigned int index) const
{
    if (index < sounds_.size())
    {
        if (ma_sound_is_playing(sounds_[index]))
        {
            ma_sound_stop(sounds_[index]);
            ma_sound_seek_to_pcm_frame(sounds_[index], 0);
        }
        ma_sound_start(sounds_[index]);
    }
}

void AudioManager::StopSound(const unsigned int index, const bool rewind) const
{
    if (index < sounds_.size())
    {
        ma_sound_stop(sounds_[index]);
        if (rewind)
        {
            ma_sound_seek_to_pcm_frame(sounds_[index], 0);
        }
    }
}

void AudioManager::ApplyVolumeChange()
{
    // TODO: Get settings from resource manager
    Settings settings;
    settings.general_volume_ = 1.0f;
    settings.music_volume_ = 0.5f;
    settings.dialogue_volume_ = 0.5f;
    settings.effects_volume_ = 0.5f;
    ma_sound_group_set_volume(sound_groups_[ESoundType::General], settings.general_volume_);
    ma_sound_group_set_volume(sound_groups_[ESoundType::Music], settings.music_volume_);
    ma_sound_group_set_volume(sound_groups_[ESoundType::Dialogue], settings.dialogue_volume_);
    ma_sound_group_set_volume(sound_groups_[ESoundType::Effect], settings.effects_volume_);
}




