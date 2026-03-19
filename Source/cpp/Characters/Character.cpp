#include "Characters/Character.hpp"

#include "glm/ext/vector_float2.hpp"

#include "Components/MovementComponent.hpp"
#include "Components/SpriteComponent.hpp"
#include "Utils/Logger.hpp"
#include "Utils/ResourceManager.hpp"

namespace
{
	constexpr float DEFAULT_SPRITE_SIZE = 100.0f;
}

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
		sprite_component_->SetScale(glm::vec2(DEFAULT_SPRITE_SIZE));
		
		movement_component_ = dynamic_cast<MovementComponent*>(resourceManager.CreateComponent(EComponentClassType::MovementComponent, componentIndex));
		RegisterComponent(movement_component_);
		
		movement_component_->SetWalkingSpeed(1.0f);
		movement_component_->SetMovementSpeed(10.0f);
		movement_component_->SetRunningSpeed(2.0f);
		movement_component_->SetSwimmingSpeed(0.5f);
		movement_component_->UseSpeedAsScales(true);
	}
	catch (...)
	{
		auto logger = Logger();
		logger.Log(ELogLevel::Error, "Character::InitializeComponents: Failed to initialize components");
	}
}
