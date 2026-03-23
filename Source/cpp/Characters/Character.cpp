#include "Characters/Character.hpp"

#include "glm/ext/vector_float2.hpp"

#include "Components/MovementComponent.hpp"
#include "Components/SpriteComponent.hpp"
#include "Components/BoxCollisionComponent.hpp"
#include "Utils/Logger.hpp"
#include "Utils/ResourceManager.hpp"

namespace
{
	constexpr float DEFAULT_SPRITE_SIZE = 100.0f;
}

Character::Character()
{
	fixed_render_list_ = false;
	InitializeComponents();
}

void Character::InitializeComponents()
{
	auto& resourceManager = ResourceManager::Instance();
	try
	{
		unsigned int componentIndex;
		sprite_component_ = dynamic_cast<SpriteComponent*>(resourceManager.CreateComponent(EComponentClassType::SpriteComponent, componentIndex, this));
		RegisterComponent(sprite_component_);
		sprite_component_->SetSize(glm::vec2(DEFAULT_SPRITE_SIZE));
		
		movement_component_ = dynamic_cast<MovementComponent*>(resourceManager.CreateComponent(EComponentClassType::MovementComponent, componentIndex, this));
		RegisterComponent(movement_component_);
		
		movement_component_->SetMovementSpeed(100.0f);
		movement_component_->SetWalkingSpeed(1.5f);
		movement_component_->SetRunningSpeed(2.0f);
		movement_component_->SetSwimmingSpeed(0.5f);
		movement_component_->UseSpeedAsScales(true);
		movement_component_->SetMovementMode(EMovementMode::Walking);
		
		// Size and position will be the same as the owner, we can adjust it by setting the scale instead
		box_collision_component_ = dynamic_cast<BoxCollisionComponent*>(resourceManager.CreateComponent(EComponentClassType::BoxCollisionComponent, componentIndex, this));
		RegisterComponent(box_collision_component_);
		box_collision_component_->SetColliderScale(glm::vec2(1.0f, 1.0f)); 
	}
	catch (...)
	{
		auto logger = Logger();
		logger.Log(ELogLevel::Error, "Character::InitializeComponents: Failed to initialize components");
	}
}
