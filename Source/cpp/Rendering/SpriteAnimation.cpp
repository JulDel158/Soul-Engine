#include "Rendering/SpriteAnimation.hpp"
#include "Utils/ResourceManager.hpp"
#include "StringGlobals.hpp"

SpriteAnimation::SpriteAnimation() :
current_time_(0.0f),
current_frame_(0),
frames_per_second_(30)
{
}

SpriteAnimation::SpriteAnimation(const std::vector<std::string>& textureNames, const unsigned int framesPerSecond) :
current_time_(0.0f),
current_frame_(0),
frames_per_second_(framesPerSecond)
{
	auto& resourceManager = ResourceManager::Instance();
	for (const auto& textureName : textureNames)
	{
		if (resourceManager.ContainsTexture2D(textureName))
		{
			textures_.push_back(resourceManager.GetTexture2D(textureName));
		}
		else
		{
			textures_.push_back(resourceManager.GetTexture2D(MISSING_TEXTURE.data()));
		}
	}
}

SpriteAnimation::~SpriteAnimation()
{
	// resource manager clears textures from memory
	textures_.clear(); 
	current_time_ = 0.0f;
	current_frame_ = 0;
	frames_per_second_ = 0;
}

void SpriteAnimation::Update(float deltaTime)
{
	current_time_ += deltaTime;
	if (textures_.empty())
	{
		return;
	}
	
	current_frame_ = (static_cast<unsigned int>(current_time_) * frames_per_second_) % static_cast<unsigned int>(textures_.size());
	//TODO: calculate index
}

void SpriteAnimation::StartAnimation(const bool restart)
{
}

void SpriteAnimation::StopAnimation(const bool restart)
{
}

void SpriteAnimation::SetAnimationSprites(const std::vector<std::string>& names)
{
}

Texture2D SpriteAnimation::GetCurrentFrameTexture() const
{
}
