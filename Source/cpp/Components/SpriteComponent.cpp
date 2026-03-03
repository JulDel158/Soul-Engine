#include "Components/SpriteComponent.hpp"

#include "Utils/ResourceManager.hpp"

#include <utility>

// SpriteComponent::SpriteComponent(std::string textureName)
// {
// 	texture_name_ = std::move(textureName);
// 	
// 	if (auto& resourceManager = ResourceManager::Instance(); resourceManager.ContainsTexture2D(texture_name_))
// 	{
// 		texture_ = resourceManager.GetTexture2D(texture_name_);
// 	}
// }

// SpriteComponent::SpriteComponent(const Texture2D& texture, std::string textureName) :
// texture_(texture),
// texture_name_(std::move(textureName))
// {
// }

void SpriteComponent::Init()
{
	BaseComponent::Init();
}

void SpriteComponent::Update(const float deltaTime)
{
	BaseComponent::Update(deltaTime);
}
