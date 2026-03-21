#ifndef BOX_COLLLISION_COMPONENT_HPP
#define BOX_COLLISION_COMPONENT_HPP
#pragma once

#include "BaseComponent.hpp"
#include "Physics/PhysicsDataStructures.hpp"

#include <tuple>
#include <optional>

class BoxCollisionComponent : public BaseComponent
{
	protected:
	using PhysicsData = std::tuple<EPhysicsObjectType, void*>;
	Quad collider;
	
	public:
	BoxCollisionComponent();
	~BoxCollisionComponent() override;
	
	void SetColliderScale(glm::vec2 scale);
	
	void Start() override;
	void Update(const float deltaTime) override;
	
	std::optional<PhysicsData> GetCollisionData() override;
};

#endif