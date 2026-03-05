#include "Components/SpriteComponent.hpp"

#include "Utils/ResourceManager.hpp"
#include "Utils/Logger.hpp"
#include "EngineDataStructures.hpp"
#include "Game/GameObject.hpp"

#include <string>
#include <tuple>

namespace
{
	constexpr auto ZERO_VECTOR_2_F = glm::vec2(0.0f);
	constexpr auto ONE_VECTOR_2_F = glm::vec2(1.0f);
	constexpr auto ONE_VECTOR_3_F = glm::vec3(1.0f);
	constexpr int BASE_SPRITE_ID = -1;
}

SpriteComponent::SpriteComponent() :
color_(ONE_VECTOR_3_F),
current_animation_(BASE_SPRITE_ID)
{
}

SpriteComponent::~SpriteComponent()
{
	animations_.clear();
}

void SpriteComponent::Init()
{
	BaseComponent::Init();
}

void SpriteComponent::Update(const float deltaTime)
{
	BaseComponent::Update(deltaTime);
	
	if (current_animation_ != BASE_SPRITE_ID && animations_.contains(current_animation_))
	{
		animations_[current_animation_].Update(deltaTime);
	}
}

void SpriteComponent::AddAnimation(const int id, const SpriteAnimation& animation)
{
	if (id == BASE_SPRITE_ID)
	{
		auto logger = Logger();
		logger.Log(ELogLevel::Warning, "SpriteComponent::AddAnimation: animation id: [" 
			+ std::to_string(id) 
			+ "] is reserved for base sprite, please use another id");
		return;
	}
	
	if (animations_.contains(id))
	{
		auto logger = Logger();
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
		auto logger = Logger();
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
	current_animation_ = BASE_SPRITE_ID;
}

std::optional<BaseComponent::RenderData> SpriteComponent::GetRenderData()
{
	if (owner_ == nullptr)
	{
		return BaseComponent::GetRenderData();
	}
	
	const glm::vec2 relativePosition = (parent_ != nullptr) ? parent_->GetPosition() : ZERO_VECTOR_2_F;
	const glm::vec2 relativeScale = (parent_ != nullptr) ? parent_->GetScale() : ONE_VECTOR_2_F;
	const float relativeRotation = (parent_ != nullptr) ? parent_->GetRotation() : 0.0f;
	
	return std::make_tuple(GetCurrentTexture(), 
		position_ + owner_->GetPosition() + relativePosition, 
		scale_ * owner_->GetSize() * relativeScale, 
		rotation_ + owner_->GetRotation() + relativeRotation, 
		color_);
}

Texture2D SpriteComponent::GetCurrentTexture()
{
	if (current_animation_ == BASE_SPRITE_ID || animations_.empty() || !animations_.contains(current_animation_))
	{
		return base_texture_;
	}
	return animations_[current_animation_].GetCurrentFrameTexture();
}
