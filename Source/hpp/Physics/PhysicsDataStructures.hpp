#ifndef PHYSICS_DATA_STRUCTURES_HPP
#define PHYSICS_DATA_STRUCTURES_HPP
#pragma once

#include "glm/glm.hpp"

struct Quad
{
	glm::vec2 position_;
	glm::vec2 size_;
	glm::vec2 scale_;
	
	Quad() : position_{glm::vec2(0.0f)}, size_{glm::vec2(0.0f)}, scale_{glm::vec2(1.0f)} {}
	
	glm::vec2 GetScaledSize() const { return size_ * scale_; }
	void GetCorners(glm::vec2& topLeft, glm::vec2& bottomLeft, glm::vec2& topRight, glm::vec2& bottomRight ) const
	{
		const float left = position_.x - (size_.x *scale_.x);
		const float right = position_.x + (size_.x *scale_.x);
		// top of the screen is represented by 0, value increases in + direction as we go down
		const float up = position_.y - (size_.y *scale_.y);
		const float down = position_.y + (size_.y *scale_.y);
		
		bottomLeft.x	= topLeft.x		= left;
		topRight.x		= bottomRight.x = right;
		
		topRight.y		= topLeft.y		= up;
		bottomRight.y	= bottomLeft.y	= down;
	}
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
	
	Triangle() : point1_{glm::vec2(0.0f)}, point2_{glm::vec2(0.0f)}, point3_{glm::vec2(0.0f)} {}
};

#endif
