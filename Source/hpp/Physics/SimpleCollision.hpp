#ifndef SIMPLE_COLLISION_HPP
#define SIMPLE_COLLISION_HPP
#pragma once

#include "glm/glm.hpp"
#include "PhysicsDataStructures.hpp"

class SimpleCollision
{
public:
	static bool IsOverlapping(const Quad& quad, const glm::vec2 point);
	static bool IsOverlapping(const Quad& a, const Quad& b);
	static bool IsOverlapping(const Circle circle, const glm::vec2 point);
	static bool IsOverlapping(const Circle a, const Circle b);
	
	SimpleCollision() = delete;
};

#endif
