#pragma once

#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"

#include "Rendering/Texture2D.hpp"

#include <tuple>
#include <optional>

// Base class for all game components
class BaseComponent
{
protected:
	using RenderData = std::tuple<Texture2D, glm::vec2, glm::vec2, float, glm::vec3>;
	friend class GameObject;
	friend class Game;
	
	glm::vec2 position_;
	glm::vec2 scale_;
	float rotation_;
	bool enabled_;
	GameObject* owner_;
	BaseComponent* parent_;
	
public:
	BaseComponent();
	virtual ~BaseComponent() = default;
	
protected:
	virtual void Start(); // Called after a level is loaded
	virtual void Update(const float deltaTime); // called each frame while game is running
	virtual void End(); // called before destroy as level is being unloaded
	
public:
	// Setters
	inline void SetPosition(const glm::vec2& position)	{ position_ = position; }
	inline void SetScale(const glm::vec2& size)			{ scale_ = size; }
	inline void SetRotation(const float rotation)		{ rotation_ = rotation; }
	inline void SetEnabled(const bool enabled)			{ enabled_ = enabled; }
	
	// Getters
	inline glm::vec2	GetPosition() const	{ return position_; }
	inline glm::vec2	GetScale() const	{ return scale_; }
	inline float		GetRotation() const	{ return rotation_; }
	inline bool			GetEnabled() const	{ return enabled_; }
	inline bool			HasOwner() const	{ return owner_ != nullptr; }
	inline bool			IsAttached() const	{ return parent_ != nullptr; }
	
	virtual std::optional<RenderData> GetRenderData(); 
};
