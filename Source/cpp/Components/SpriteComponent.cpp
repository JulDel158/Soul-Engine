#include "Components/SpriteComponent.hpp"

#include "Utils/ResourceManager.hpp"
#include "Utils/Logger.hpp"
#include "StringGlobals.hpp"
#include "EngineDataStructures.hpp"
#include "Game/GameObject.hpp"

#include <string>
#include <tuple>

SpriteComponent::SpriteComponent() :
current_animation_(-1),
color_(glm::vec3(1.0f))
{
}

SpriteComponent::~SpriteComponent()
{
	animations_.clear();
}

void SpriteComponent::AddAnimation(const int id, const SpriteAnimation& animation)
{
	if (animations_.contains(id))
	{
		auto logger = Logger(LOG_PATH.data());
		logger.Log(ELogLevel::Warning, "SpriteComponent::AddAnimation: animation: [" 
			+ std::to_string(id) 
			+ "] already present, replacing it with new animation");
	}
	
	animations_[id] = animation;
}

void SpriteComponent::PlayAnimation(const int id, const bool playOnce, const bool restart)
{
	if (!animations_.contains(id))
	{
		auto logger = Logger(LOG_PATH.data());
		logger.Log(ELogLevel::Warning, "SpriteComponent::PlayAnimation: trying to play invalid animation: [" 
			+ std::to_string(id)
			+ "]");
		return;
	}
	StopCurrentAnimation();
	current_animation_ = id;
	animations_[id].StartAnimation(playOnce, restart);
}

void SpriteComponent::StopCurrentAnimation()
{
	if (animations_.contains(current_animation_))
	{
		animations_[current_animation_].StopAnimation();
	}
	current_animation_ = -1;
}

void SpriteComponent::Init()
{
	BaseComponent::Init();
}

void SpriteComponent::Update(const float deltaTime)
{
	BaseComponent::Update(deltaTime);
	
	if (current_animation_ != -1 && animations_.contains(current_animation_))
	{
		animations_[current_animation_].Update(deltaTime);
	}
	
	if (owner_ != nullptr)
	{
		owner_->AddToRenderList(std::make_tuple(GetCurrentTexture(), 
			position_ + owner_->position_, 
			size_ * owner_->size_, 
			rotation_ + owner_->rotation_, 
			color_));
	}
}

Texture2D SpriteComponent::GetCurrentTexture()
{
	if (current_animation_ == -1 || animations_.empty() || !animations_.contains(current_animation_))
	{
		return base_texture_;
	}
	return animations_[current_animation_].GetCurrentFrameTexture();
}
