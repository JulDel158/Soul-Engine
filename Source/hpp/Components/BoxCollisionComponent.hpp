#ifndef BOX_COLLLISION_COMPONENT_HPP
#define BOX_COLLISION_COMPONENT_HPP
#pragma once

#include "BaseComponent.hpp"
#include "Physics/PhysicsDataStructures.hpp"
#include "glm/ext/vector_float2.hpp"

#include <tuple>
#include <optional>
#include <functional>

class BoxCollisionComponent : public BaseComponent
{
	protected:
	using PhysicsData = std::tuple<EPhysicsObjectType, const void*>;
	Quad collider_;
	glm::vec2 cached_owner_position_;
	bool overlap_;
	
	std::function<void(BaseComponent*)> function_on_collision_;
	
	public:
	BoxCollisionComponent();
	~BoxCollisionComponent() override;
	
	// Setters
	void SetColliderScale(glm::vec2 scale);
	void SetOverlap(const bool overlap) { overlap_ = overlap; }
	
	void BindReleased(const std::function<void(BaseComponent*)>& lambda);
	
	// Getters
	Quad GetCollider() const	{ return collider_; }
	bool GetOverlap() const		{ return overlap_; }
	
	void Start() override;
	void Update(const float deltaTime) override;
	
	std::optional<PhysicsData> GetCollisionData() const override;
};

#endif