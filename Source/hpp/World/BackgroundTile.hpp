#ifndef BACKGROUND_TILE_HPP
#define BACKGROUND_TILE_HPP
#pragma once

#include "Game/GameObject.hpp"
#include "Components/SpriteComponent.hpp"
#include "Rendering/Texture2D.hpp"

#include <vector>

class BackgroundTile : public GameObject
{
protected:
	SpriteComponent *sprite_;
public:
	BackgroundTile();
	explicit BackgroundTile(std::vector<Texture2D>& textures);
	virtual ~BackgroundTile() override;
	
protected:
	void InitializeComponents();
};


#endif