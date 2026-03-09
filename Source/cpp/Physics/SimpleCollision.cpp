#include "Physics/SimpleCollision.hpp"

namespace
{
	constexpr unsigned int TOP_LEFT = 0;
	constexpr unsigned int BOTTOM_LEFT = 1;
	constexpr unsigned int TOP_RIGHT = 2;
	constexpr unsigned int BOTTOM_RIGHT = 3;
}

bool SimpleCollision::IsOverlapping(const Quad& quad, const glm::vec2 point)
{
	const bool xInRange = (point.x >= quad.position_.x - quad.GetScaledSize().x) &&  point.x <= quad.position_.x + quad.GetScaledSize().x;
	const bool yInRange = (point.y >= quad.position_.y - quad.GetScaledSize().y) &&  point.y <= quad.position_.y + quad.GetScaledSize().y;
	return xInRange && yInRange;
}

bool SimpleCollision::IsOverlapping(const Quad& a, const Quad& b)
{
	glm::vec2 cornersA[4];
	glm::vec2 cornersB[4];
	a.GetCorners(cornersA[TOP_LEFT], cornersA[BOTTOM_LEFT], cornersA[TOP_RIGHT], cornersA[BOTTOM_RIGHT]);
	b.GetCorners(cornersB[TOP_LEFT], cornersB[BOTTOM_LEFT], cornersB[TOP_RIGHT], cornersB[BOTTOM_RIGHT]);
	
	bool result = false;
	for (int i = 0; i < 4; ++i)
	{
		// we check all corners for both quads as one of them could be completely inside the other
		if (IsOverlapping(b, cornersA[i]) || IsOverlapping(a, cornersB[i]))
		{
			result = true;
			break;
		}
	}
	
	return result;
}

bool SimpleCollision::IsOverlapping(const Circle circle, const glm::vec2 point)
{
	const float x = point.x - circle.position_.x;
	const float y = point.y - circle.position_.y;
	const float distanceSquared = (x * x) + (y * y);
	return distanceSquared <= circle.radius_ * circle.radius_;
}

bool SimpleCollision::IsOverlapping(const Circle a, const Circle b)
{
	const float x = a.position_.x - b.position_.x;
	const float y = a.position_.y - b.position_.y;
	const float distanceSquared = (x * x) + (y * y);
	return distanceSquared <= (a.radius_ + b.radius_) * (a.radius_ + b.radius_);
}


