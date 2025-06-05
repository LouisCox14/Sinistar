#pragma once
#include "Collisions.h"

std::optional<Weave::Physics::Collision> Weave::Physics::Collides(Weave::Physics::Collider lhs, Weave::Physics::Collider rhs)
{
	if (!(lhs.layerMask & rhs.layers).any()) return { };

	return std::visit([](auto&& a, auto&& b) { return Collides(a, b); }, lhs.shape, rhs.shape);
}

std::optional<Weave::Physics::Collision> Weave::Physics::Collides(Shapes::Rectangle lhs, Shapes::Rectangle rhs)
{
	if (lhs.max.x < rhs.min.x || lhs.min.x > rhs.max.x || lhs.max.y < rhs.min.y || lhs.min.y > rhs.max.y) return { };

	Mathematics::Vector2<float> lhsPos = (lhs.min + lhs.max) * 0.5f;
	Mathematics::Vector2<float> rhsPos = (rhs.min + rhs.max) * 0.5f;

	Mathematics::Vector2<float> lhsExtents = (lhs.max - lhs.min) * 0.5f;
	Mathematics::Vector2<float> rhsExtents = (rhs.max - rhs.min) * 0.5f;

	Mathematics::Vector2<float> connectingVector = lhsPos - rhsPos;
	Mathematics::Vector2<float> penetrationVector = (lhsExtents + rhsExtents) - Mathematics::Abs(connectingVector);

	if (Mathematics::Abs(penetrationVector.x) < Mathematics::Abs(penetrationVector.y))
	{
		return Collision(Mathematics::Vector2<float>((float)std::copysign(1, connectingVector.x), 0), Mathematics::Abs(penetrationVector.x));
	}
	else
	{
		return Collision(Mathematics::Vector2<float>(0, (float)std::copysign(1, connectingVector.y)), Mathematics::Abs(penetrationVector.y));
	}
}

std::optional<Weave::Physics::Collision> Weave::Physics::Collides(Shapes::Rectangle lhs, Shapes::Circle rhs)
{
	Mathematics::Vector2<float> closestPoint = { Mathematics::Clamp(rhs.center.x, lhs.min.x, lhs.max.x), Mathematics::Clamp(rhs.center.y, lhs.min.y, lhs.max.y) };
	Mathematics::Vector2<float> connectingVector = rhs.center - closestPoint;
	
	float distance = connectingVector.GetMagnitude();

	if (distance > rhs.radius) return { };

	Mathematics::Vector2<float> normal;

	if (distance == 0.0f) // Deals with circle center being inside the rect.
	{
		float distances[4] = {
			rhs.center.x - lhs.min.x,     // Right
			lhs.max.x - rhs.center.x,     // Left
			lhs.max.y - rhs.center.y,     // Bottom
			rhs.center.y - lhs.min.y      // Top
		};

		int minIndex = 0;
		float minValue = distances[0];

		for (int i = 1; i < 4; ++i)
		{
			if (distances[i] < minValue)
			{
				minValue = distances[i];
				minIndex = i;
			}
		}

		const Mathematics::Vector2<float> normals[4] = {
			Mathematics::Vector2<float>(1,  0),  // Right
			Mathematics::Vector2<float>(-1,  0),  // Left
			Mathematics::Vector2<float>(0,  -1),  // Bottom
			Mathematics::Vector2<float>(0, 1)   // Top
		};

		return Collision(normals[minIndex], rhs.radius);
	}

	if (Mathematics::Abs(connectingVector.x) > Mathematics::Abs(connectingVector.y))
	{
		normal = Mathematics::Vector2<float>(-(float)std::copysign(1, connectingVector.x), 0);
	}
	else
	{
		normal = Mathematics::Vector2<float>(0, -(float)std::copysign(1, connectingVector.y));
	}

	return Collision(normal, rhs.radius - distance);
}

std::optional<Weave::Physics::Collision> Weave::Physics::Collides(Shapes::Rectangle lhs, Shapes::Line rhs)
{
	if (Collides(rhs.start, lhs)) return Collision();
	if (Collides(rhs.end, lhs)) return Collision();

	if (Weave::Physics::Collides(Weave::Shapes::Line(lhs.min, { lhs.min.x, lhs.max.y }), rhs)) return Collision();
	if (Weave::Physics::Collides(Weave::Shapes::Line({ lhs.min.x, lhs.max.y }, lhs.max), rhs)) return Collision();
	if (Weave::Physics::Collides(Weave::Shapes::Line(lhs.max, { lhs.max.x, lhs.min.y }), rhs)) return Collision();
	if (Weave::Physics::Collides(Weave::Shapes::Line({ lhs.max.x, lhs.min.y }, lhs.min), rhs)) return Collision();

	return { };

	// TODO: Get collision depth and normal.
}

std::optional<Weave::Physics::Collision> Weave::Physics::Collides(Shapes::Circle lhs, Shapes::Circle rhs)
{
	Weave::Mathematics::Vector2<float> connectingVector = lhs.center - rhs.center;

	float combinedRadius = lhs.radius + rhs.radius;
	float distance = connectingVector.GetSquaredMagnitude();

	if (distance > combinedRadius * combinedRadius) return { };
	if (distance == 0.0f) return Collision(Mathematics::Vector2<float>(0.0f, 1.0f), lhs.radius); // Deals with the case of exactly aligned collisions.

	distance = connectingVector.GetMagnitude();

	Collision collision;

	collision.depth = combinedRadius - distance;
	collision.normal = connectingVector.GetUnitVector();

	return collision;
}

std::optional<Weave::Physics::Collision> Weave::Physics::Collides(Shapes::Circle lhs, Shapes::Line rhs)
{
	// Check the ends of the line collider first. 
	{
		std::optional<Weave::Physics::Collision> pointCheck = Collides(rhs.start, lhs);
		if (pointCheck) return pointCheck;

		pointCheck = Collides(rhs.end, lhs);
		if (pointCheck) return pointCheck;
	}

	Weave::Mathematics::Vector2<float> lineDirection = rhs.end - rhs.start;
	Weave::Mathematics::Vector2<float> circleToStart = lhs.center - rhs.start;

	float t = Weave::Mathematics::Vector2<float>::Dot(circleToStart, lineDirection) / Weave::Mathematics::Vector2<float>::Dot(lineDirection, lineDirection);
	t = Mathematics::Clamp(t, 0.0f, 1.0f);

	Weave::Mathematics::Vector2<float> closestPoint = rhs.start + lineDirection * t;
	Weave::Mathematics::Vector2<float> distance = closestPoint - lhs.center;

	if (distance.GetMagnitude() > lhs.radius) return { };

	return Collision();

	// TODO: Get collision depth and normal.
}

std::optional<Weave::Physics::Collision> Weave::Physics::Collides(Shapes::Line lhs, Shapes::Line rhs)
{
	float denom = (lhs.end.x - lhs.start.x) * (rhs.end.y - rhs.start.y) - (lhs.end.y - lhs.start.y) * (rhs.end.x - rhs.start.x);
	if (denom == 0.0f) return { }; // No need to check in the case of parallell lines.

	float ua = ((rhs.end.x - rhs.start.x) * (lhs.start.y - rhs.start.y) - (rhs.end.y - rhs.start.y) * (lhs.start.x - rhs.start.x)) / denom;
	float ub = ((lhs.end.x - lhs.start.x) * (lhs.start.y - rhs.start.y) - (lhs.end.y - lhs.start.y) * (lhs.start.x - rhs.start.x)) / denom;

	if (!(ua >= 0.0f && ua <= 1.0f && ub >= 0.0f && ub <= 1.0f)) return { };

	return Collision();

	// TODO: Get collision depth and normal.
}

std::optional<Weave::Physics::Collision> Weave::Physics::Collides(Mathematics::Vector2<float> point, Shapes::Rectangle collider)
{
	if (point.x < collider.min.x || point.x > collider.max.x || point.y < collider.min.y || point.y > collider.max.y) return { };

	return Collision();

	// TODO: Get collision depth and normal.
}

std::optional<Weave::Physics::Collision> Weave::Physics::Collides(Mathematics::Vector2<float> point, Shapes::Circle collider)
{
	if ((point - collider.center).GetMagnitude() > collider.radius) return { };

	return Collision();

	// TODO: Get collision depth and normal.
}

std::optional<Weave::Physics::Collision> Weave::Physics::Collides(Shapes::Circle lhs, Shapes::Rectangle rhs)
{
	std::optional<Collision> collision = Weave::Physics::Collides(rhs, lhs);

	if (!collision) return { };

	collision.value().normal *= -1;

	return collision;
}

std::optional<Weave::Physics::Collision> Weave::Physics::Collides(Shapes::Line lhs, Shapes::Rectangle rhs)
{
	std::optional<Collision> collision = Weave::Physics::Collides(rhs, lhs);

	if (!collision) return { };

	collision.value().normal *= -1;

	return collision;
}

std::optional<Weave::Physics::Collision> Weave::Physics::Collides(Shapes::Line lhs, Shapes::Circle rhs)
{
	std::optional<Collision> collision = Weave::Physics::Collides(rhs, lhs);

	if (!collision) return { };

	collision.value().normal *= -1;

	return collision;
}

Weave::Physics::Collider Weave::Physics::TranslateCollider(Collider collider, Mathematics::Vector2<float> offset)
{
	collider.shape = std::visit([&offset](auto&& a) { return TranslateCollider(a, offset); }, collider.shape).shape;
	return collider;
}

Weave::Physics::Collider Weave::Physics::TranslateCollider(Shapes::Rectangle collider, Mathematics::Vector2<float> offset)
{
	collider.min += offset;
	collider.max += offset;
	return Weave::Physics::Collider(collider);
}

Weave::Physics::Collider Weave::Physics::TranslateCollider(Shapes::Circle collider, Mathematics::Vector2<float> offset)
{
	collider.center += offset;
	return Weave::Physics::Collider(collider);
}

Weave::Physics::Collider Weave::Physics::TranslateCollider(Shapes::Line collider, Mathematics::Vector2<float> offset)
{
	collider.start += offset;
	collider.end += offset;
	return Weave::Physics::Collider(collider);
}