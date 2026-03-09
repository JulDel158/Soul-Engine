#ifndef PHYSICS_DATA_STRUCTURES_HPP
#define PHYSICS_DATA_STRUCTURES_HPP
#pragma once

#include "glm/glm.hpp"

struct Quad
{
	glm::vec2 position_;
	glm::vec2 size_;
	glm::vec2 scale_;
	float rotation_;
	
	Quad() : position_{glm::vec2(0.0f)}, size_{glm::vec2(0.0f)}, scale_{glm::vec2(1.0f)}, rotation_{0.0f} {}
};

struct Line
{
	glm::vec2 point1_;
	glm::vec2 point2_;
	
	Line() : point1_{glm::vec2(0.0f)}, point2_{glm::vec2(0.0f)} {}
};

struct Circle
{
	glm::vec2 position_;
	float radius_;
	
	Circle() : position_{glm::vec2(0.0f)}, radius_{0.0f} {}
};

struct Triangle
{
	glm::vec2 point1_;
	glm::vec2 point2_;
	glm::vec2 point3_;
	float rotation_;
	
	Triangle() : point1_{glm::vec2(0.0f)}, point2_{glm::vec2(0.0f)}, point3_{glm::vec2(0.0f)}, rotation_ {0.0f} {}
};

#endif
