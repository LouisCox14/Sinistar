#pragma once
#include "Physics.h"
#include "Quadtree.h"

bool Weave::Physics::Collides(Weave::Physics::Collider lhs, Weave::Physics::Collider rhs)
{
	return std::visit([](auto&& a, auto&& b) { return Collides(a, b); }, lhs.shape, rhs.shape);
}

bool Weave::Physics::Collides(Shapes::Rectangle lhs, Shapes::Rectangle rhs)
{
	return (lhs.min.x < rhs.max.x && lhs.max.x > rhs.min.x && lhs.min.y < rhs.max.y && lhs.max.y > rhs.min.y);
}

bool Weave::Physics::Collides(Shapes::Rectangle lhs, Shapes::Circle rhs)
{
	Weave::Mathematics::Vector2<float> closestPoint = { Weave::Mathematics::Clamp(rhs.center.x, lhs.min.x, lhs.max.x), Weave::Mathematics::Clamp(rhs.center.y, lhs.min.y, lhs.max.y) };
	Weave::Mathematics::Vector2<float> distance = rhs.center - closestPoint;

	return distance.GetMagnitude() <= rhs.radius;
}

bool Weave::Physics::Collides(Shapes::Rectangle lhs, Shapes::Line rhs)
{
	if (Collides(rhs.start, lhs)) return true;
	if (Collides(rhs.end, lhs)) return true;

	if (Weave::Physics::Collides(Weave::Shapes::Line(lhs.min, { lhs.min.x, lhs.max.y }), rhs)) return true;
	if (Weave::Physics::Collides(Weave::Shapes::Line({ lhs.min.x, lhs.max.y }, lhs.max), rhs)) return true;
	if (Weave::Physics::Collides(Weave::Shapes::Line(lhs.max, { lhs.max.x, lhs.min.y }), rhs)) return true;
	if (Weave::Physics::Collides(Weave::Shapes::Line({ lhs.max.x, lhs.min.y }, lhs.min), rhs)) return true;

	return false;
}

bool Weave::Physics::Collides(Shapes::Circle lhs, Shapes::Circle rhs)
{
	Weave::Mathematics::Vector2<float> distance = lhs.center - rhs.center;

	return distance.GetMagnitude() <= (lhs.radius + rhs.radius);
}

bool Weave::Physics::Collides(Shapes::Circle lhs, Shapes::Line rhs)
{
	if (Collides(rhs.start, lhs)) return true;
	if (Collides(rhs.end, lhs)) return true;

	Weave::Mathematics::Vector2<float> lineDirection = rhs.end - rhs.start;
	Weave::Mathematics::Vector2<float> circleToStart = lhs.center - rhs.start;

	float t = Weave::Mathematics::Vector2<float>::Dot(circleToStart, lineDirection) / Weave::Mathematics::Vector2<float>::Dot(lineDirection, lineDirection);
	t = Mathematics::Clamp(t, 0.0f, 1.0f);

	Weave::Mathematics::Vector2<float> closestPoint = rhs.start + lineDirection * t;
	Weave::Mathematics::Vector2<float> distance = closestPoint - lhs.center;

	return distance.GetMagnitude() <= lhs.radius;
}

bool Weave::Physics::Collides(Shapes::Line lhs, Shapes::Line rhs)
{
	float denom = (lhs.end.x - lhs.start.x) * (rhs.end.y - rhs.start.y) - (lhs.end.y - lhs.start.y) * (rhs.end.x - rhs.start.x);
	if (denom == 0.0f) return false;

	float ua = ((rhs.end.x - rhs.start.x) * (lhs.start.y - rhs.start.y) - (rhs.end.y - rhs.start.y) * (lhs.start.x - rhs.start.x)) / denom;
	float ub = ((lhs.end.x - lhs.start.x) * (lhs.start.y - rhs.start.y) - (lhs.end.y - lhs.start.y) * (lhs.start.x - rhs.start.x)) / denom;

	return (ua >= 0.0f && ua <= 1.0f && ub >= 0.0f && ub <= 1.0f);
}

bool Weave::Physics::Collides(Mathematics::Vector2<float> point, Shapes::Rectangle collider)
{
	return (point.x >= collider.min.x && point.x <= collider.max.x && point.y >= collider.min.y && point.y <= collider.max.y);
}

bool Weave::Physics::Collides(Mathematics::Vector2<float> point, Shapes::Circle collider)
{
	return (point - collider.center).GetMagnitude() <= collider.radius;
}

bool Weave::Physics::Collides(Shapes::Circle lhs, Shapes::Rectangle rhs)
{
	return Weave::Physics::Collides(rhs, lhs);
}

bool Weave::Physics::Collides(Shapes::Line lhs, Shapes::Rectangle rhs)
{
	return Weave::Physics::Collides(rhs, lhs);
}

bool Weave::Physics::Collides(Shapes::Line lhs, Shapes::Circle rhs)
{
	return Weave::Physics::Collides(rhs, lhs);
}

Weave::Physics::Collider Weave::Physics::TranslateCollider(Collider collider, Mathematics::Vector2<float> offset)
{
	return std::visit([&offset](auto&& a) { return TranslateCollider(a, offset); }, collider.shape);
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

Weave::Physics::PhysicsHandler::PhysicsHandler(float _gravity) : gravity(_gravity) { }

void Weave::Physics::PhysicsHandler::HandlePhysics(ECS::World& world)
{
	QuadTree quadtree({{-10.0f, -10.0f}, {10.0f, 10.0f}}, 5);

	for (auto [entity, transform, rigidbody] : world.GetView<Transform, Rigidbody>())
	{
		quadtree.Insert(entity, Weave::Physics::TranslateCollider(rigidbody.collider, transform.position));
	}

	for (auto [entity, transform, rigidbody] : world.GetView<Transform, Rigidbody>())
	{
		Weave::Physics::Collider translatedCollider = Weave::Physics::TranslateCollider(rigidbody.collider, transform.position);

		std::vector<std::pair<Weave::ECS::EntityID, Collider>> found = quadtree.Query(translatedCollider);

		if (entity == 0) std::cout << found.size() << " colliders tested.\n";

		for (const auto& [otherEntity, otherCollider] : found)
		{
			if (otherEntity != entity && Collides(translatedCollider, otherCollider))
			{
				if (entity == 0) std::cout << "Collision detected | Entities: [" << entity << ", " << otherEntity << "]\n";
			}
		}
	}
}
