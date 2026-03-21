#include "World/BackgroundTile.hpp"

#include "Utils/ResourceManager.hpp"
#include "EngineDataStructures.hpp"
#include "Utils/Logger.hpp"
#include "Rendering/SpriteAnimation.hpp"

BackgroundTile::BackgroundTile()
{
	InitializeComponents();
}

BackgroundTile::BackgroundTile(std::vector<Texture2D>& textures) :
sprite_(nullptr)
{
	InitializeComponents();
	
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

BackgroundTile::~BackgroundTile()
{
	
}

void BackgroundTile::InitializeComponents()
{
	auto& resourceManager = ResourceManager::Instance();
	try
	{
		unsigned int componentIndex;
		sprite_ = dynamic_cast<SpriteComponent*>(resourceManager.CreateComponent(EComponentClassType::SpriteComponent, componentIndex, this));
		RegisterComponent(sprite_);
	}
	catch (...)
	{
		auto logger = Logger();
		logger.Log(ELogLevel::Error, "BackgroundTile::InitializeComponents: Failed to initialize components");
	}
}
