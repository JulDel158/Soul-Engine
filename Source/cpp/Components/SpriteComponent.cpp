#include "Components/SpriteComponent.hpp"

#include "Utils/ResourceManager.hpp"
#include "Utils/Logger.hpp"
#include "StringGlobals.hpp"
#include "EngineDataStructures.hpp"

#include <string>

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

void SpriteComponent::PlayAnimation(const int id)
{
	if (!animations_.contains(id))
	{
		auto logger = Logger(LOG_PATH.data());
		logger.Log(ELogLevel::Warning, "SpriteComponent::PlayAnimation: trying to play invalid animation: [" 
			+ std::to_string(id)
			+ "]");
		return;
	}
	if (animations_.contains(current_animation_))
	{
		animations_[current_animation_].StopAnimation();
	}
	current_animation_ = id;
	animations_[id].StartAnimation();
	
}

void SpriteComponent::Init()
{
	BaseComponent::Init();
}

void SpriteComponent::Update(const float deltaTime)
{
	BaseComponent::Update(deltaTime);
}
