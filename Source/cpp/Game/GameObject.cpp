#include "Game/GameObject.hpp"

#include "Components/BaseComponent.hpp"

#include <optional>

namespace
{
	constexpr auto ZERO_VECTOR_2_F = glm::vec2(0.0f);
	constexpr auto ONE_HUNDRED_VECTOR_2_F = glm::vec2(100.0f);
}

GameObject::GameObject() :
	position_(ZERO_VECTOR_2_F),
	size_(ONE_HUNDRED_VECTOR_2_F),
	rotation_(0.0f), 
	type_(EGameObjectClassType::Base),
	is_active_(true),
	is_visible_(true),
	fixed_render_list_(true)
{
}

void GameObject::Clear()
{
	// Components are created in the resource manager, therefore they are deallocated there
	components_.clear();
}

void GameObject::AddComponent(BaseComponent* component)
{
	components_.push_back(component);
	component->owner_ = this;
}

void GameObject::Start()
{
	for (auto& component : components_)
	{
		component->Start();
	}
}

void GameObject::Update(const float deltaTime)
{
	// we will flush the render list before updating components, that way components may repopulate it if necessary
	if (!fixed_render_list_)
	{
		render_list_.clear();
		render_list_.reserve(components_.size());
	}
	
	for (const auto& component : components_)
	{
		component->Update(deltaTime);
		if (!fixed_render_list_)
		{
			continue;
		}
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
