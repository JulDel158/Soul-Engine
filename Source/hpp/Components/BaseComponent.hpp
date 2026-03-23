#ifndef BASE_COMPONENT_HPP
#define BASE_COMPONENT_HPP
#pragma once

#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"

#include "Rendering/Texture2D.hpp"
#include "EngineDataStructures.hpp"
#include "Physics/PhysicsDataStructures.hpp"

#include <tuple>
#include <optional>

#include "Game/GameObject.hpp"

// Base class for all game components
class BaseComponent
{
protected:
	using RenderData = std::tuple<Texture2D, glm::vec2, glm::vec2, float, glm::vec3>;
	using PhysicsData = std::tuple<EPhysicsObjectType, const void*>;
	friend class GameObject;
	friend class Game;
	friend class ResourceManager;
	
	glm::vec2 position_;
	glm::vec2 size_;
	float rotation_;
	bool enabled_;
	bool is_visible_;
	GameObject* owner_;
	BaseComponent* parent_;
	EComponentClassType component_type_;
	
public:
	BaseComponent();
	virtual ~BaseComponent() = default;
	
protected:
	virtual void Start(); // Called after a level is loaded
	virtual void Update(const float deltaTime); // called each frame while game is running
	virtual void End(); // called before destroy as level is being unloaded
	
	virtual void Clear(); // called right before getting destroyed
	
public:
	// Setters
	inline void SetPosition(const glm::vec2& position)				{ position_ = position; }
	inline void SetSize(const glm::vec2& size)						{ size_ = size; }
	inline void SetRotation(const float rotation)					{ rotation_ = rotation; }
	inline void SetEnabled(const bool enabled)						{ enabled_ = enabled; }
	inline void SetIsVisible(const bool visible)					{ is_visible_ = visible; }
	inline void SetComponentType(EComponentClassType componentType)	{ component_type_ = componentType; }
	
	// Getters
	glm::vec2					GetPosition() const;
	glm::vec2					GetSize() const;
	float						GetRotation() const;
	inline bool					IsEnabled() const		{ return enabled_; }
	inline bool					IsVisible() const		{ return is_visible_; }
	inline bool					HasOwner() const		{ return owner_ != nullptr; }
	inline bool					IsAttached() const		{ return parent_ != nullptr; }
	inline EComponentClassType	GetType() const			{ return component_type_; }
	inline GameObject*			GetOwner() const		{ return owner_; }
	
	virtual std::optional<RenderData> GetRenderData(); 
	virtual std::optional<PhysicsData> GetCollisionData() const;
};
#endif