#include "Components/BaseComponent.hpp"

BaseComponent::BaseComponent() : 
position_(glm::vec2(0.0f)),
scale_(glm::vec2(1.0f)),
rotation_(0.0f),
enabled_(true),
owner_(nullptr),
parent_(nullptr)
{
}

void BaseComponent::Start()
{
}

void BaseComponent::Update(const float deltaTime)
{
}

void BaseComponent::End()
{
}

void BaseComponent::Clear()
{
}

std::optional<BaseComponent::RenderData> BaseComponent::GetRenderData()
{
	return std::nullopt;
}

std::optional<BaseComponent::PhysicsData> BaseComponent::GetCollisionData() const
{
	return std::nullopt;
}
