#ifndef SIMPLE_COLLISION_HPP
#define SIMPLE_COLLISION_HPP
#pragma once

#include "glm/glm.hpp"
#include "PhysicsDataStructures.hpp"

#include "robin_hood_hash/robin_hood.h"

class BaseComponent;

class PhysicsEngine
{
private:
	robin_hood::unordered_set<BaseComponent*> registered_components_;
	
public:
	static PhysicsEngine& Instance();
	
	void RegisterComponent(BaseComponent& component);
	void UnregisterComponent(BaseComponent& component);
	
	static bool IsOverlapping(const Quad& quad, const glm::vec2 point);
	static bool IsOverlapping(const Quad& a, const Quad& b);
	static bool IsOverlapping(const Circle circle, const glm::vec2 point);
	static bool IsOverlapping(const Circle a, const Circle b);
	
	PhysicsEngine(const PhysicsEngine&) = delete;
	PhysicsEngine& operator=(const PhysicsEngine&) = delete;
	
private:
	PhysicsEngine() = default;
};

#endif
