#include "Components/BaseComponent.hpp"

BaseComponent::BaseComponent() :
	position_(glm::vec2(0.0f)),
	size_(glm::vec2(1.0f)),
	rotation_(0.0f),
	enabled_(true),
	is_visible_(true),
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

glm::vec2 BaseComponent::GetPosition() const
{
	const glm::vec2 parentPosition = (parent_ != nullptr ? parent_->GetPosition() : glm::vec2(0.0f));
	// if parent is not null, owner's position was included in the line above via parent's GetPosition(); in which case, we do not want to add it again
	// same if owner is null for some reason, we do not try to access the owner's GetPosition() function.
	const glm::vec2 ownerPosition = ( parent_ != nullptr || owner_ == nullptr) ? glm::vec2(0.0f) : owner_->GetPosition();
	return position_ + parentPosition + ownerPosition; // relative position of this component to its parent and owner
}

glm::vec2 BaseComponent::GetSize() const
{
	const glm::vec2 parentSize = (parent_ != nullptr) ? parent_->GetSize() : glm::vec2(0.0f);
	// if parent is not null, owner's size was included in the line above via parent's GetSize(); in which case, we do not want to add it again
	// same if owner is null for some reason, we do not try to access the owner's GetSize() function.
	const glm::vec2 ownerSize = (parent_ != nullptr || owner_ == nullptr) ? glm::vec2(0.0f) : owner_->GetSize();
	return size_ + parentSize + ownerSize;
}

float BaseComponent::GetRotation() const
{
	const float parentRotation = (parent_ != nullptr) ? parent_->GetRotation() : 0.0f;
	// if parent is not null, owner's rotation was included in the line above via parent's GetRotation(); in which case, we do not want to add it again
	// same if owner is null for some reason, we do not try to access the owner's GetRotation() function.
	const float ownerRotation = (parent_ != nullptr || owner_ == nullptr) ? 0.0f : owner_->GetRotation();
	return rotation_ + parentRotation + ownerRotation;
}

std::optional<BaseComponent::RenderData> BaseComponent::GetRenderData()
{
	return std::nullopt;
}

std::optional<BaseComponent::PhysicsData> BaseComponent::GetCollisionData() const
{
	return std::nullopt;
}
