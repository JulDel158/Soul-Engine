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
	
	bool CheckCollision(BaseComponent& component, BaseComponent*& outHit);
	
	// Check whether the 2 objects are colliding
	static bool IsOverlapping(const Quad& quad, const glm::vec2 point);
	static bool IsOverlapping(const Quad& a, const Quad& b);
	static bool IsOverlapping(const Circle circle, const glm::vec2 point);
	static bool IsOverlapping(const Circle a, const Circle b);
	static bool IsOverlapping(const Quad& a, const Circle b);
	static bool IsOverlapping(const Quad& a, const Line b);
	static bool SegmentCircleCollision(const Line a, const Circle circle);
	static bool IsPointOnSegment(const Line a, glm::vec2 point);
	static int Orientation(glm::vec2 p, glm::vec2 q, glm::vec2 r);
	static bool DoSegmentsIntersect(const Line a, const Line b);
	
	PhysicsEngine(const PhysicsEngine&) = delete;
	PhysicsEngine& operator=(const PhysicsEngine&) = delete;
	
private:
	PhysicsEngine() = default;
};

#endif
