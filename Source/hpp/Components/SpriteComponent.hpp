#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include "BaseComponent.hpp"
#include "Rendering/SpriteAnimation.hpp"
#include "Rendering/Texture2D.hpp"

#include <unordered_map>

class SpriteComponent : public BaseComponent
{
protected:
	std::unordered_map<int,SpriteAnimation> animations_;
	Texture2D base_texture_;
	int current_animation_;
	
public:
	SpriteComponent()=default;
	~SpriteComponent()override;
	
	void AddAnimation(const int id, const SpriteAnimation& animation);
	void PlayAnimation(const int id);
	
	//TODO: provide way to supply game renderer with sprite
	
protected:
	void Init() override;
	void Update(const float deltaTime) override;
};



#endif
