#pragma once

#include "glm/ext/vector_float3.hpp"

#include "BaseComponent.hpp"
#include "Rendering/SpriteAnimation.hpp"
#include "Rendering/Texture2D.hpp"

#include <unordered_map>

class GameObject;

class SpriteComponent : public BaseComponent
{
protected:
	std::unordered_map<int,SpriteAnimation> animations_;
	glm::vec3 color_;
	int current_animation_;
	Texture2D base_texture_;
	
public:
	SpriteComponent();
	~SpriteComponent()override;
	
protected:
	void Start() override;
	void Update(const float deltaTime) override;
	
public:
	void AddAnimation(const int id, const SpriteAnimation& animation);
	void PlayAnimation(const int id, const bool playOnce = false, const bool restart = true);
	void StopCurrentAnimation();
	
	inline void SetDefaultTexture(const Texture2D& texture) { base_texture_ = texture; }
	
	std::optional<RenderData> GetRenderData() override;
	
private:
	Texture2D GetCurrentTexture();
};
