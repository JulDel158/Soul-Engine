#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include "BaseComponent.hpp"
#include "Rendering/Texture2D.hpp"

#include <string>
#include <vector>

class SpriteComponent : public BaseComponent
{
protected:
	std::vector<Texture2D> textures_;
	std::vector<std::string> texture_names_;
	
public:
	SpriteComponent()=default;
	//explicit SpriteComponent(std::string textureName);
	//explicit SpriteComponent(const Texture2D& texture, std::string textureName = "");
	~SpriteComponent()override=default;
	
	
	
protected:
	void Init() override;
	void Update(const float deltaTime) override;
};



#endif
