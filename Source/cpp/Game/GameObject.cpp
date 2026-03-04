#include "Game/GameObject.hpp"

#include "Components/BaseComponent.hpp"

namespace
{
	constexpr auto ZERO_VECTOR_2F = glm::vec2(0.0f);
}

GameObject::GameObject() :
position_(ZERO_VECTOR_2F),
size_(ZERO_VECTOR_2F),
rotation_(0.0f)
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
	
	for (const auto& component : components_)
	{
		// Note: some components will repopulate the render list as it updates, the game loop processes rendering after updates
		// therefore we will have the correct render data for this frame
		component->Update(deltaTime);
	}
}
