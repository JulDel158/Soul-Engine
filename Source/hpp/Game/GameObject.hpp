#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP
#pragma once

#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"

#include "Rendering/Texture2D.hpp"

#include <tuple>
#include <vector>

class BaseComponent;

// This class represents the base form of all objects that are part of the game,
// The game class will have a list of all loaded objects and send updates as well as process rendering for any sprites they contain
class GameObject
{
protected:
	using RenderData = std::tuple<Texture2D, glm::vec2, glm::vec2, float, glm::vec3>;
	using RenderList = std::vector<RenderData>;
	friend class Game;
	
	glm::vec2 position_;
	glm::vec2 size_;
	RenderList render_list_;
	std::vector<BaseComponent*> components_;
	float rotation_;
	bool is_active_;
	bool is_visible_;
	
public:
	GameObject();
	virtual ~GameObject();
	
protected:
	virtual void Start(); // called at the start of the game (after loading)
	virtual void Update(const float deltaTime); // called each frame while game is running
	virtual void End(); // called before unloading (before this item gets destroyed)
	
public:
	
	void AddComponent(BaseComponent* component);
	
	// Setters
	inline void SetPosition(const glm::vec2& position)	{position_ = position;}
	inline void SetSize(const glm::vec2& size)			{size_ = size;}
	inline void SetRotation(const float rotation)		{rotation_ = rotation;}
	inline void SetIsActive(const bool isActive)		{ is_active_ = isActive; }
	inline void SetIsVisible(const bool isVisible)		{ is_visible_ = isVisible; }
	
	// Getters
	inline glm::vec2	GetPosition() const			{ return position_; }
	inline glm::vec2	GetSize() const				{ return size_; }
	inline float		GetRotation() const 		{ return rotation_; }
	inline const RenderList& GetRenderList() const	{ return render_list_; }
	inline bool			IsActive() const			{ return is_active_; }
	inline bool			IsVisible() const			{ return is_visible_; }
};
#endif