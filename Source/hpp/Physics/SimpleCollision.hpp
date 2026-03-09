#ifndef SIMPLE_COLLISION_HPP
#define SIMPLE_COLLISION_HPP
#pragma once

#include "glm/glm.hpp"
#include "PhysicsDataStructures.hpp"

namespace SimpleCollision
{
	bool IsOverlapping(Quad quad, const glm::vec2 point);
	bool IsOverlapping(Quad quad, Quad other);
}

#endif
