#include "Components/BoxCollisionComponent.hpp"

#include "EngineDataStructures.hpp"
#include "Game/GameObject.hpp"
#include "Physics/PhysicsEngine.hpp"

BoxCollisionComponent::BoxCollisionComponent()
{
	overlap_ = false;
	function_on_collision_ = nullptr;
	component_type_ = EComponentClassType::BoxCollisionComponent;
	collider_.scale_ = glm::vec2(1.0f);
	is_visible_ = false;
	
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
	collider_.scale_ = scale;
}

void BoxCollisionComponent::BindReleased(const std::function<void(BaseComponent*)>& lambda)
{
	function_on_collision_ = lambda;
}

void BoxCollisionComponent::Start()
{
	BaseComponent::Start();
	
	collider_.position_ = this->GetPosition();
	collider_.size_ = this->GetSize();
	
	cached_owner_position_ = (owner_ != nullptr) ? owner_->GetPosition() : glm::vec2(0.0f);
}

void BoxCollisionComponent::Update(const float deltaTime)
{
	BaseComponent::Update(deltaTime);
	
	// relative position and size
	collider_.position_ = this->GetPosition();
	collider_.size_ = this->GetSize();
	
	// Now we check collision
	auto& physicsEngine = PhysicsEngine::Instance();
	
	if (BaseComponent* outHit = nullptr; physicsEngine.CheckCollision(*this, outHit) && owner_ != nullptr)
	{
		if (!overlap_)
		{
			owner_->SetPosition(cached_owner_position_);
		}
		
		if (function_on_collision_ != nullptr)
		{
			function_on_collision_(outHit);
		}
	}
	
	if (owner_ != nullptr)
	{
		cached_owner_position_ = owner_->GetPosition();
	}
}

std::optional<BaseComponent::PhysicsData> BoxCollisionComponent::GetCollisionData() const 
{
	return std::make_tuple(EPhysicsObjectType::Quad, &collider_);
}
