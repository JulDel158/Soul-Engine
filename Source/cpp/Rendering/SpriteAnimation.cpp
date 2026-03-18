#include "Rendering/SpriteAnimation.hpp"

#include "Utils/ResourceManager.hpp"
#include "EngineDataStructures.hpp"
#include "Utils/Logger.hpp"

#include <utility>

SpriteAnimation::SpriteAnimation() :
	current_time_(0.0f),
	current_frame_(0),
	frames_per_second_(20),
	is_playing_(false),
	play_once_(false), 
	auto_play_(false)
{
}

SpriteAnimation::SpriteAnimation(const std::vector<ESpriteKey>& textureNames, const unsigned int framesPerSecond, const bool autoPlay) :
current_time_(0.0f),
current_frame_(0),
frames_per_second_(framesPerSecond),
is_playing_(false),
play_once_(false),
auto_play_(autoPlay)
{
	SetAnimationSprites(textureNames);
}

SpriteAnimation::SpriteAnimation(std::vector<Texture2D>& textures, const unsigned int framesPerSecond, const bool autoPlay) :
textures_(std::move(textures)),
current_time_(0.0f),
current_frame_(0),
frames_per_second_(framesPerSecond),
is_playing_(false),
play_once_(false),
auto_play_(autoPlay)
{
}

SpriteAnimation::~SpriteAnimation()
{
	// resource manager clears textures from memory
	textures_.clear(); 
	current_time_ = 0.0f;
	current_frame_ = 0;
	frames_per_second_ = 0;
	is_playing_ = false;
	play_once_ = false;
}

void SpriteAnimation::Update(const float deltaTime)
{
	current_time_ += deltaTime;
	if (textures_.empty())
	{
		return;
	}
	
	if (auto_play_)
	{
		StartAnimation();
	}
	
	current_frame_ = static_cast<unsigned int>(current_time_ * static_cast<float>(frames_per_second_)) % static_cast<unsigned int>(textures_.size());
	
	// once the final frame is reached the full animation has played, stop
	if (play_once_ && current_frame_ == textures_.size() - 1)
	{
		StopAnimation();
	}
}

void SpriteAnimation::StartAnimation(const bool playOnce, const bool restart)
{
	is_playing_ = true;
	play_once_ = playOnce;
	if (restart)
	{
		Restart();
	}
}

void SpriteAnimation::StopAnimation(const bool restart)
{
	is_playing_ = false;
	play_once_ = false;
	if (restart)
	{
		Restart();
	}
}

void SpriteAnimation::SetAnimationSprites(const std::vector<ESpriteKey>& names)
{
	auto& resourceManager = ResourceManager::Instance();
	auto logger = Logger();
	textures_.resize(names.size());
	for (const auto& textureName : names)
	{
		if (resourceManager.ContainsTexture2D(textureName))
		{
			textures_.emplace_back(resourceManager.GetTexture2D(textureName));
		}
		else
		{
			textures_.emplace_back(resourceManager.GetTexture2D(ESpriteKey::Missing));
			logger.Log(ELogLevel::Warning, "SpriteAnimation::SetAnimationSprites: Texture: [" 
				+ DataConverter::ToString(textureName) 
				+ "] Missing!\nLoading MISSING_TEXTURE into frame: ["
				+ std::to_string(textures_.size())
				+ "]");
		}
	}
}

Texture2D SpriteAnimation::GetCurrentFrameTexture() const
{
	if (textures_.empty() || current_frame_ >= textures_.size())
	{
		auto logger = Logger();
		auto& resourceManager = ResourceManager::Instance();
		logger.Log(ELogLevel::Error, "SpriteAnimation::GetCurrentFrameTexture: invalid texture! Is array empty: [" + 
			std::to_string(textures_.empty()) + 
			"] current frame: [" + 
			std::to_string(current_frame_) +
			"]");
		return resourceManager.GetTexture2D(ESpriteKey::Missing);
	}
	
	return textures_[current_frame_];
}

