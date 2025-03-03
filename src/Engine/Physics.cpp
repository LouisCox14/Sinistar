#pragma once
#include "Physics.h"

bool Weave::Physics::Collides(Weave::Physics::Collider lhs, Weave::Physics::Collider rhs)
{
	return std::visit([](auto&& a, auto&& b) { return Collides(a, b); }, lhs.shape, rhs.shape);
}

bool Weave::Physics::Collides(Rectangle lhs, Rectangle rhs)
{
	return (lhs.min.x < rhs.max.x && lhs.max.x > rhs.min.x && lhs.min.y < rhs.max.y && lhs.max.y > rhs.min.y);
}

bool Weave::Physics::Collides(Rectangle lhs, Circle rhs)
{
	Weave::Mathematics::Vector2<float> closestPoint = { Weave::Mathematics::Clamp(rhs.center.x, lhs.min.x, lhs.max.x), Weave::Mathematics::Clamp(rhs.center.y, lhs.min.y, lhs.max.y) };
	Weave::Mathematics::Vector2<float> distance = rhs.center - closestPoint;

	return distance.GetMagnitude() <= rhs.radius;
}

bool Weave::Physics::Collides(Rectangle lhs, Line rhs)
{

	if (Weave::Physics::Collides(Weave::Physics::Line(lhs.min, { lhs.min.x, lhs.max.y }), rhs)) return true;
	if (Weave::Physics::Collides(Weave::Physics::Line({ lhs.min.x, lhs.max.y }, lhs.max), rhs)) return true;
	if (Weave::Physics::Collides(Weave::Physics::Line(lhs.max, { lhs.max.x, lhs.min.y }), rhs)) return true;
	if (Weave::Physics::Collides(Weave::Physics::Line({ lhs.max.x, lhs.min.y }, lhs.min), rhs)) return true;

	return false;
}

bool Weave::Physics::Collides(Circle lhs, Circle rhs)
{
	Weave::Mathematics::Vector2<float> distance = lhs.center - rhs.center;

	return distance.GetMagnitude() <= (lhs.radius + rhs.radius);
}

bool Weave::Physics::Collides(Circle lhs, Line rhs)
{
	// NOT YET IMPLEMENTED
	return false;
}

bool Weave::Physics::Collides(Line lhs, Line rhs)
{
	float denom = (lhs.end.x - lhs.start.x) * (rhs.end.y - rhs.start.y) - (lhs.end.y - lhs.start.y) * (rhs.end.x - rhs.start.x);
	if (denom == 0.0f) return false;

	float ua = ((rhs.end.x - rhs.start.x) * (lhs.start.y - rhs.start.y) - (rhs.end.y - rhs.start.y) * (lhs.start.x - rhs.start.x)) / denom;
	float ub = ((lhs.end.x - lhs.start.x) * (lhs.start.y - rhs.start.y) - (lhs.end.y - lhs.start.y) * (lhs.start.x - rhs.start.x)) / denom;

	return (ua >= 0.0f && ua <= 1.0f && ub >= 0.0f && ub <= 1.0f);
}

bool Weave::Physics::Collides(Circle lhs, Rectangle rhs)
{
	return Weave::Physics::Collides(rhs, lhs);
}

bool Weave::Physics::Collides(Line lhs, Rectangle rhs)
{
	return Weave::Physics::Collides(rhs, lhs);
}

bool Weave::Physics::Collides(Line lhs, Circle rhs)
{
	return Weave::Physics::Collides(rhs, lhs);
}
