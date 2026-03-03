#include "Audio/AudioManager.hpp"

#include <ranges>

#include "StringGlobals.hpp"
#include "Utils/ResourceManager.hpp"
#include "Utils/Logger.hpp"

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
    for (auto& sound : sounds_)
    {
        ma_sound_uninit(sound);
        delete sound;
        sound = nullptr;
    }
    sounds_.clear();
    
    for (ma_sound_group*& soundGroup : sound_groups_ | std::views::values)
    {
        ma_sound_group_uninit(soundGroup);
        delete soundGroup;
        soundGroup = nullptr;
    }
    sound_groups_.clear();
    
    ma_engine_uninit(audio_engine_);
    delete audio_engine_;
}

AudioManager& AudioManager::Instance()
{
    static auto instance = AudioManager();
    return instance;
}

void AudioManager::Initialize()
{
	auto logger = Logger(LOG_PATH.data(), ELogLevel::Info);
    if (const ma_result result = ma_engine_init(nullptr, audio_engine_); result != MA_SUCCESS) {
    	logger.Log(ELogLevel::Error,"AudioManager: Failed to initialize the sound engine. result: " + std::to_string(static_cast<int>(result)));
    	return;
    }
    
    if (ma_sound_group_init(audio_engine_, 0 ,nullptr ,sound_groups_[ESoundType::General]) != MA_SUCCESS)
    {
        logger.Log(ELogLevel::Error,"AudioManager: Failed to initialize the sound group: ESoundType::General");
    }
    
    if (ma_sound_group_init(audio_engine_, 0 ,sound_groups_[ESoundType::General] ,sound_groups_[ESoundType::Music]) != MA_SUCCESS)
    {
        logger.Log(ELogLevel::Error, "AudioManager: Failed to initialize the sound group: ESoundType::Music");
    }
    
    if (ma_sound_group_init(audio_engine_, 0 ,sound_groups_[ESoundType::General] ,sound_groups_[ESoundType::Effect]) != MA_SUCCESS)
    {
       logger.Log(ELogLevel::Error, "AudioManager: Failed to initialize the sound group: ESoundType::Effect");
    }
    
    if (ma_sound_group_init(audio_engine_, 0 ,sound_groups_[ESoundType::General] ,sound_groups_[ESoundType::Dialogue]) != MA_SUCCESS)
    {
        logger.Log(ELogLevel::Error, "AudioManager: Failed to initialize the sound group: ESoundType::Dialogue");
    }
    
    ApplyVolumeChange();
    
	// TEMPORARY
    LoadSound(AUDIO_1.data(), ESoundType::Music);
}

int AudioManager::LoadSound(const std::string& filename, const ESoundType soundType)
{
	auto logger = Logger(LOG_PATH.data(), ELogLevel::Info);
    if (audio_engine_ == nullptr)
    {
    	logger.Log(ELogLevel::Error, "AudioManager::LoadSound: No audio engine?!");
        return -1;
    }
    
     ma_sound* sound = new ma_sound();
    
    if (ma_sound_init_from_file(audio_engine_, filename.c_str(), 
        0, sound_groups_[soundType], nullptr, sound) != MA_SUCCESS) 
    {
    	logger.Log(ELogLevel::Error, "AudioManager::LoadSound: Failed to load sound from file: " + filename);
        return -1;
    }

    sounds_.push_back(sound);
    
    // return type is int
    return static_cast<int>(sounds_.size() - 1);
}

void AudioManager::PlaySound(const unsigned int index,  const bool rewind) const
{
    if (index < sounds_.size())
    {
        if (ma_sound_is_playing(sounds_[index]))
        {
            ma_sound_stop(sounds_[index]);
        }
        
        if (rewind)
        {
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
    const ResourceManager& resourceManager = ResourceManager::Instance();
    const Settings settings = resourceManager.GetSettings();
    ma_sound_group_set_volume(sound_groups_[ESoundType::General], settings.general_volume_);
    ma_sound_group_set_volume(sound_groups_[ESoundType::Music], settings.music_volume_);
    ma_sound_group_set_volume(sound_groups_[ESoundType::Dialogue], settings.dialogue_volume_);
    ma_sound_group_set_volume(sound_groups_[ESoundType::Effect], settings.effects_volume_);
}




