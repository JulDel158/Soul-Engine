#include "Characters/Character.hpp"

#include "Components/MovementComponent.hpp"
#include "Components/SpriteComponent.hpp"
#include "Utils/Logger.hpp"
#include "Utils/ResourceManager.hpp"

Character::Character()
{
	InitializeComponents();
}

void Character::InitializeComponents()
{
	auto& resourceManager = ResourceManager::Instance();
	try
	{
		unsigned int componentIndex;
		sprite_component_ = dynamic_cast<SpriteComponent*>(resourceManager.CreateComponent(EComponentClassType::SpriteComponent, componentIndex));
		RegisterComponent(sprite_component_);
		
		movement_component_ = dynamic_cast<MovementComponent*>(resourceManager.CreateComponent(EComponentClassType::MovementComponent, componentIndex));
		RegisterComponent(movement_component_);
	}
	catch (...)
	{
		auto logger = Logger();
		logger.Log(ELogLevel::Error, "Character::InitializeComponents: Failed to initialize components");
	}
}
