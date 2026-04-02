#include "World/BackgroundTile.hpp"

#include "glm/ext/vector_float2.hpp"

#include "EngineDataStructures.hpp"
#include "Utils/ResourceManager.hpp"
#include "Utils/Logger.hpp"
#include "Rendering/SpriteAnimation.hpp"
#include "Components/SpriteComponent.hpp"
#include "Components/BoxCollisionComponent.hpp"

BackgroundTile::BackgroundTile()
{
	InitializeComponents();
}

void BackgroundTile::SetSprite(std::vector<Texture2D>& textures) const
{
	if (textures.empty() || sprite_ == nullptr)
	{
		auto l = Logger();
		l.Log(ELogLevel::Warning, "BackgroundTile(): textures array size: [" + 
			std::to_string(textures.size()) + 
			"] sprite_ is nullptr = [" +
			std::to_string(sprite_ == nullptr) + "]");
		return;
	}
	if (textures.size() == 1)
	{
		sprite_->SetDefaultTexture(textures.at(0));
	}
	else
	{
		SpriteAnimation spriteAnimation = SpriteAnimation(textures, 30, true);
		sprite_->AddAnimation(0, spriteAnimation);
		sprite_->PlayAnimation(0);
	}
}

void BackgroundTile::InitializeComponents()
{
	auto& resourceManager = ResourceManager::Instance();
	try
	{
		unsigned int componentIndex;
		sprite_ = resourceManager.CreateComponent<SpriteComponent>(componentIndex, this);
		
		collider_ = resourceManager.CreateComponent<BoxCollisionComponent>(componentIndex, this);
		collider_->SetColliderScale(glm::vec2(0.8f, 0.8f));
		collider_->SetOverlap(true);
	}
	catch (...)
	{
		auto logger = Logger();
		logger.Log(ELogLevel::Error, "BackgroundTile::InitializeComponents: Failed to initialize components");
	}
}
