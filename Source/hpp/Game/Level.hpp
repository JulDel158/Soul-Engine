#ifndef LEVEL_HPP
#define LEVEL_HPP
#pragma once

#include <map>
#include <vector>

class GameObject;
class SpriteRenderer;

class Level
{
protected:
	friend class Game;
	std::map<unsigned int, std::vector<GameObject*>> game_objects_;
	
public:
	Level();
	virtual ~Level();
	
protected:
	virtual void Start();
	virtual void Update(const float deltaTime);
	virtual void End();
	virtual void Render(const float deltaTime, SpriteRenderer& renderer);
};


#endif