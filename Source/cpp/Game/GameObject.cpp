#include "Game/GameObject.hpp"

#include "Components/BaseComponent.hpp"

#include <optional>

namespace
{
	constexpr auto ZERO_VECTOR_2F = glm::vec2(0.0f);
	constexpr auto ONE_HUNDRED_VECTOR_2F = glm::vec2(100.0f);
}

GameObject::GameObject() :
position_(ZERO_VECTOR_2F),
size_(ONE_HUNDRED_VECTOR_2F),
rotation_(0.0f),
is_active_(true),
is_visible_(true)
{
}

GameObject::~GameObject()
{
	// whoever creates the components should be in charge of deleting them
	components_.clear();
}

void GameObject::AddComponent(BaseComponent* component)
{
	components_.push_back(component);
	component->owner_ = this;
}

void GameObject::Init()
{
	for (auto& component : components_)
	{
		component->Init();
	}
}

void GameObject::Update(const float deltaTime)
{
	// we will flush the render list before updating components, that way components may repopulate it if necessary
	render_list_.clear();
	render_list_.reserve(components_.size());
	
	for (const auto& component : components_)
	{
		component->Update(deltaTime);
		if (auto renderData = component->GetRenderData(); renderData != std::nullopt)
		{
			render_list_.emplace_back(renderData.value());
		}
	}
}

void GameObject::End()
{
	for (const auto& component : components_)
	{
		component->End();
	}
}
