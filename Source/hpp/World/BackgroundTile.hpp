#ifndef BACKGROUND_TILE_HPP
#define BACKGROUND_TILE_HPP
#pragma once

#include "Game/GameObject.hpp"
#include "Rendering/Texture2D.hpp"

#include <vector>

class SpriteComponent;
class BoxCollisionComponent;

class BackgroundTile : public GameObject
{
protected:
	SpriteComponent* sprite_;
	BoxCollisionComponent* collider_;
	
public:
	BackgroundTile();
	//explicit BackgroundTile(std::vector<Texture2D>& textures);
	virtual ~BackgroundTile() override = default;
	
	void SetSprite(std::vector<Texture2D>& textures) const;
	
protected:
	void InitializeComponents();
};


#endif