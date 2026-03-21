#include "Components/BoxCollisionComponent.hpp"

#include "EngineDataStructures.hpp"
#include "Game/GameObject.hpp"
#include "Physics/PhysicsEngine.hpp"

BoxCollisionComponent::BoxCollisionComponent()
{
	component_type_ = EComponentClassType::BoxCollisionComponent;
	collider.scale_ = glm::vec2(1.0f);
	
	auto& physicsEngine = PhysicsEngine::Instance();
	physicsEngine.RegisterComponent(*this);
}

BoxCollisionComponent::~BoxCollisionComponent()
{
	auto& physicsEngine = PhysicsEngine::Instance();
	physicsEngine.UnregisterComponent(*this);
}

void BoxCollisionComponent::SetColliderScale(const glm::vec2 scale)
{
	collider.scale_ = scale;
}

void BoxCollisionComponent::Start()
{
	BaseComponent::Start();
	
	collider.position_ = this->GetPosition();
	collider.size_ = this->GetSize();
}

void BoxCollisionComponent::Update(const float deltaTime)
{
	BaseComponent::Update(deltaTime);
	
	collider.position_ = this->GetPosition();
	collider.size_ = this->GetSize();
}

std::optional<BaseComponent::PhysicsData> BoxCollisionComponent::GetCollisionData() 
{
	return std::make_tuple(EPhysicsObjectType::Quad, &collider);
}
