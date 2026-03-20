#include "Physics/PhysicsEngine.hpp"

#include "glm/gtx/norm.inl"

#include "Components/BaseComponent.hpp"
#include "EngineDataStructures.hpp"

#include <optional>
#include <limits>
#include <tuple>

namespace
{
	constexpr unsigned int TOP_LEFT = 0;
	constexpr unsigned int BOTTOM_LEFT = 1;
	constexpr unsigned int TOP_RIGHT = 2;
	constexpr unsigned int BOTTOM_RIGHT = 3;
	
	constexpr int COLLINEAR = 0;
	constexpr int CLOCKWISE = 1;
	constexpr int COUNTERCLOCKWISE = -1;
	
	constexpr unsigned int TYPE = 0;
	constexpr unsigned int DATA = 1;
}

PhysicsEngine& PhysicsEngine::Instance()
{
	static auto instance = PhysicsEngine();
	return instance;
}

void PhysicsEngine::RegisterComponent(BaseComponent& component)
{
	registered_components_.insert(&component);
}

void PhysicsEngine::UnregisterComponent(BaseComponent& component)
{
	if (registered_components_.contains(&component))
	{
		registered_components_.erase(&component);
	}
}

bool PhysicsEngine::CheckCollision(const BaseComponent& component, BaseComponent*& outHit)
{
	auto collisionData = component.GetCollisionData();
	if (collisionData == std::nullopt)
	{
		// the component had no collision data...
		return false;
	}
	void* data = std::get<DATA>(collisionData.value());
	for (const auto& collider : registered_components_)
	{
		auto otherCollisionData = collider->GetCollisionData();
		if (&component == collider || collider->GetOwner() == component.GetOwner() || otherCollisionData == std::nullopt)
		{
			continue;
		}
		
		bool foundCollision = false;
		void* otherData = std::get<DATA>(otherCollisionData.value());
		
		// check and reinterpret data
		// demonic switch statement of pain and suffering
		switch (std::get<TYPE>(collisionData.value()))
		{
		case EPhysicsObjectType::Line:
			{
				const Line* lineA = static_cast<Line*>(data);
				switch (std::get<TYPE>(otherCollisionData.value()))
				{
				case EPhysicsObjectType::Line:
					foundCollision = DoSegmentsIntersect(*lineA, *static_cast<Line*>(otherData));
					break;
				case EPhysicsObjectType::Point:
					foundCollision = IsPointOnSegment(*lineA, *static_cast<glm::vec2*>(otherData));
					break;
				case EPhysicsObjectType::Quad:
					foundCollision = IsOverlapping(*static_cast<Quad*>(otherData), *lineA);
					break;
				case EPhysicsObjectType::Circle:
					foundCollision = SegmentCircleCollision(*lineA, *static_cast<Circle*>(otherData));
					break;
				case EPhysicsObjectType::None:
				default: // NOLINT
					continue;
				}
				break;
			}
		case EPhysicsObjectType::Point:
			{
				const glm::vec2* pointA = static_cast<glm::vec2*>(data);
				switch (std::get<TYPE>(otherCollisionData.value()))
				{
				case EPhysicsObjectType::Line:
					foundCollision = IsPointOnSegment(*static_cast<Line*>(otherData), *pointA);
					break;
				case EPhysicsObjectType::Point:
					foundCollision = *static_cast<glm::vec2*>(otherData) == *pointA;
					break;
				case EPhysicsObjectType::Quad:
					foundCollision = IsOverlapping(*static_cast<Quad*>(otherData), *pointA);
					break;
				case EPhysicsObjectType::Circle:
					foundCollision = IsOverlapping(*static_cast<Circle*>(otherData), *pointA);
					break;
				case EPhysicsObjectType::None:
				default: // NOLINT
					continue;
				}
				break;
			}
		case EPhysicsObjectType::Quad:
			{
				const Quad* quadA = static_cast<Quad*>(data);
				switch (std::get<TYPE>(otherCollisionData.value()))
				{
				case EPhysicsObjectType::Line:
					foundCollision = IsOverlapping(*quadA,*static_cast<Line*>(otherData));
					break;
				case EPhysicsObjectType::Point:
					foundCollision = IsOverlapping(*quadA,*static_cast<glm::vec2*>(otherData));
					break;
				case EPhysicsObjectType::Quad:
					foundCollision = IsOverlapping(*quadA,*static_cast<Quad*>(otherData));
					break;
				case EPhysicsObjectType::Circle:
					foundCollision = IsOverlapping(*quadA, *static_cast<Circle*>(otherData));
					break;
				case EPhysicsObjectType::None:
				default: // NOLINT
					continue;
				}
				break;
			}
		case EPhysicsObjectType::Circle:
			{
				const Circle* circleA = static_cast<Circle*>(data);
				switch (std::get<TYPE>(otherCollisionData.value()))
				{
				case EPhysicsObjectType::Line:
					foundCollision = SegmentCircleCollision(*static_cast<Line*>(otherData), *circleA);
					break;
				case EPhysicsObjectType::Point:
					foundCollision = IsOverlapping(*circleA, *static_cast<glm::vec2*>(otherData));
					break;
				case EPhysicsObjectType::Quad:
					foundCollision = IsOverlapping(*static_cast<Quad*>(otherData), *circleA);
					break;
				case EPhysicsObjectType::Circle:
					foundCollision = IsOverlapping(*circleA, *static_cast<Circle*>(otherData));
					break;
				case EPhysicsObjectType::None:
				default: // NOLINT
					continue;
				}
				break;
			}
		case EPhysicsObjectType::None:
		default: // NOLINT
			return false;
		}
		
		if (foundCollision)
		{
			outHit = collider;
			return true;
		}
	}
	
	return false;
}

bool PhysicsEngine::IsOverlapping(const Quad& quad, const glm::vec2 point)
{
	const bool xInRange = (point.x >= quad.position_.x - quad.GetScaledSize().x) &&  point.x <= quad.position_.x + quad.GetScaledSize().x;
	const bool yInRange = (point.y >= quad.position_.y - quad.GetScaledSize().y) &&  point.y <= quad.position_.y + quad.GetScaledSize().y;
	return xInRange && yInRange;
}

bool PhysicsEngine::IsOverlapping(const Quad& a, const Quad& b)
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

bool PhysicsEngine::IsOverlapping(const Circle circle, const glm::vec2 point)
{
	const float x = point.x - circle.position_.x;
	const float y = point.y - circle.position_.y;
	const float distanceSquared = (x * x) + (y * y);
	return distanceSquared <= circle.radius_ * circle.radius_;
}

bool PhysicsEngine::IsOverlapping(const Circle a, const Circle b)
{
	const float x = a.position_.x - b.position_.x;
	const float y = a.position_.y - b.position_.y;
	const float distanceSquared = (x * x) + (y * y);
	return distanceSquared <= (a.radius_ + b.radius_) * (a.radius_ + b.radius_);
}

bool PhysicsEngine::IsOverlapping(const Quad& a, const Circle b)
{
	bool result = false;
	glm::vec2 cornersA[4];
	a.GetCorners(cornersA[TOP_LEFT], cornersA[BOTTOM_LEFT], cornersA[TOP_RIGHT], cornersA[BOTTOM_RIGHT]);
	
	// All the lines that can be formed from the quad
	const auto topLeftAndRight = Line(cornersA[TOP_LEFT], cornersA[TOP_RIGHT]);
	const auto topLeftAndBottom = Line(cornersA[TOP_LEFT], cornersA[BOTTOM_LEFT]);
	const auto topLeftAndBottomRight = Line(cornersA[TOP_LEFT], cornersA[BOTTOM_RIGHT]);
	const auto bottomLeftAndTopRight = Line(cornersA[BOTTOM_LEFT], cornersA[TOP_RIGHT]);
	const auto bottomRightAndLeft = Line(cornersA[BOTTOM_RIGHT], cornersA[BOTTOM_LEFT]);
	const auto bottomRightAndTop = Line(cornersA[BOTTOM_RIGHT], cornersA[TOP_RIGHT]);
	
	result = SegmentCircleCollision(topLeftAndRight, b) ||
				SegmentCircleCollision(topLeftAndBottom, b) ||
				SegmentCircleCollision(topLeftAndBottomRight, b) ||
				SegmentCircleCollision(bottomLeftAndTopRight, b) ||
				SegmentCircleCollision(bottomRightAndLeft, b) ||
				SegmentCircleCollision(bottomRightAndTop, b);
	
	return result;
}

bool PhysicsEngine::IsOverlapping(const Quad& a, const Line b)
{
	// check if any of the outer lines of the quad overlap with line b, 
	// otherwise, check if any of the point resides inside the quad
	bool result = false;
	
	glm::vec2 cornersA[4];
	a.GetCorners(cornersA[TOP_LEFT], cornersA[BOTTOM_LEFT], cornersA[TOP_RIGHT], cornersA[BOTTOM_RIGHT]);
	
	// The outer lines that can be formed from the quad
	const auto topLeftAndRight = Line(cornersA[TOP_LEFT], cornersA[TOP_RIGHT]);
	const auto topLeftAndBottom = Line(cornersA[TOP_LEFT], cornersA[BOTTOM_LEFT]);
	const auto bottomRightAndLeft = Line(cornersA[BOTTOM_RIGHT], cornersA[BOTTOM_LEFT]);
	const auto bottomRightAndTop = Line(cornersA[BOTTOM_RIGHT], cornersA[TOP_RIGHT]);
	
	result = DoSegmentsIntersect(topLeftAndRight, b) ||
				DoSegmentsIntersect(topLeftAndBottom, b) ||
				DoSegmentsIntersect(bottomRightAndLeft, b) ||
				DoSegmentsIntersect(bottomRightAndTop, b) ||
				IsOverlapping(a, b.point1_) || 
				IsOverlapping(a, b.point2_);
	
	return result;
}

// Function to check if a line segment A intersects a circle with center and radius of circle
bool PhysicsEngine::SegmentCircleCollision(const Line a, const Circle circle) {
	// Vector a.point1 -> a.point2
	const glm::vec2 aVector = a.point2_ - a.point1_;

	// Vector a.point1 -> circle.position
	const glm::vec2 aToCircle = circle.position_ - a.point1_;

	// Project AC onto AB, computing the parameter t of the projection
	const float aLenghtSquare = glm::length2(aVector);
	if (aLenghtSquare < std::numeric_limits<float>::epsilon()) {
		// A and B are the same point — check if it's inside the circle
		return glm::length2(aToCircle) <= circle.radius_ * circle.radius_;
	}

	float t = (aToCircle.x * aVector.x + aToCircle.y * aVector.y) / aLenghtSquare;
	t = glm::clamp(t, 0.0f, 1.0f);

	// Find the closest point on the segment to the circle center
	const auto closest = glm::vec2( a.point1_.x + t * aVector.x, a.point1_.y + t * aVector.y );

	// Check if the closest point is within the circle
	return glm::length2(circle.position_ - closest) <= circle.radius_ * circle.radius_;
}

bool PhysicsEngine::IsPointOnSegment(const Line a, glm::vec2 point)
{
	return point.x <= glm::max(a.point1_.x, a.point2_.x) &&
			point.x >= glm::min(a.point1_.x, a.point2_.x) &&
			point.y <= glm::max(a.point1_.y, a.point2_.y) &&
			point.y >= glm::min(a.point1_.y, a.point2_.y);
}

// function to find orientation of ordered triplet (p, q, r)
// 0 --> p, q and r are collinear
// 1 --> Clockwise
// -1 --> Counterclockwise
int PhysicsEngine::Orientation(const glm::vec2 p, const glm::vec2 q, const glm::vec2 r) {
	float value = (q.y - p.y) * (r.x - q.x) -
			  (q.x - p.x) * (r.x - q.y);

	// collinear
	if (value == 0.0f)
	{
		return COLLINEAR;
	}

	// clock or counterclockwise wise
	// 1 for clockwise, -1 for counterclockwise
	return (value > 0) ? CLOCKWISE : COUNTERCLOCKWISE;
}

// function to check if two line segments intersect
bool PhysicsEngine::DoSegmentsIntersect(const Line a, const Line b) {

	// find the four orientations needed
	// for general and special cases
	const int o1 = Orientation(a.point1_, a.point2_, b.point1_);
	const int o2 = Orientation(a.point1_, a.point2_, b.point2_);
	const int o3 = Orientation(b.point1_, b.point2_, a.point1_);
	const int o4 = Orientation(b.point1_, b.point2_, a.point2_);

	// general case
	if (o1 != o2 && o3 != o4)
	{
		return true;
	}

	// special cases
	// p1, q1 and p2 are collinear and p2 lies on segment p1q1
	if (o1 == COLLINEAR && IsPointOnSegment(a, b.point1_))
	{
		return true;
	}

	// p1, q1 and q2 are collinear and q2 lies on segment p1q1
	if (o2 == COLLINEAR && IsPointOnSegment(a, b.point2_))
	{
		return true;
	}

	// p2, q2 and p1 are collinear and p1 lies on segment p2q2
	if (o3 == COLLINEAR && IsPointOnSegment(b, a.point1_))
	{
		return true;
	}

	// p2, q2 and q1 are collinear and q1 lies on segment p2q2 
	if (o4 == COLLINEAR && IsPointOnSegment(b, a.point2_))
	{
		return true;
	}

	return false;
}