#include "Components/BaseComponent.hpp"

BaseComponent::BaseComponent() : 
position_(glm::vec2(0.0f)),
size_(glm::vec2(1.0f)),
rotation_(0.0f),
enabled_(true),
owner_(nullptr),
parent_(nullptr)
{
}

void BaseComponent::Init()
{
}

void BaseComponent::Update(const float deltaTime)
{
}
