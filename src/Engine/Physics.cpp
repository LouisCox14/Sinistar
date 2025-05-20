#pragma once
#include "Physics.h"
#include <iostream>

void Weave::Physics::PhysicsHandler::UpdateCollisions(ECS::World& world)
{
	std::queue<ECS::EntityID> componentRemovalQueue;

	for (auto [entity, colliding] : world.GetView<Colliding>())
	{
		std::queue<ECS::EntityID> collisionRemovalQueue;

		for (std::map<ECS::EntityID, CollisionData>::iterator collision = colliding.collisions.begin(); collision != colliding.collisions.end(); collision++)
		{
			if (collision->second.state == CollisionState::Ended)
			{
				collisionRemovalQueue.push(collision->first);
				continue;
			}

			collision->second.state = CollisionState::Ended;
		}

		while (!collisionRemovalQueue.empty())
		{
			colliding.collisions.erase(collisionRemovalQueue.front());
			collisionRemovalQueue.pop();
		}

		if (colliding.collisions.size() == 0)
		{
			componentRemovalQueue.push(entity);
		}
	}

	while (!componentRemovalQueue.empty())
	{
		world.RemoveComponent<Colliding>(componentRemovalQueue.front());
		componentRemovalQueue.pop();
	}
}


void Weave::Physics::PhysicsHandler::CheckCollisions(ECS::World& world)
{
	Weave::ECS::WorldView<Transform, Rigidbody> view = world.GetView<Transform, Rigidbody>();

	QuadTree quadtree({ {-100.0f, -100.0f}, {100.0f, 100.0f} }, 5, 5);
	quadtree.Reserve(view.GetEntityCount());

	for (auto [entity, transform, rigidbody] : view)
	{
		quadtree.Insert(entity, Weave::Physics::TranslateCollider(rigidbody.collider, transform.position));
	}

	Weave::Utilities::ThreadPool collisionThreadPool(8);
	std::vector<std::future<std::vector<Weave::Physics::EntityCollision>>> futures;

	for (Weave::QuadTree::NodeID nodeID : quadtree.GetAllLeafNodes())
	{
		std::vector<std::pair<ECS::EntityID, Physics::Collider>> objects = quadtree.GetLeafObjects(nodeID);

		if (objects.size() == 0) continue;

		futures.push_back(collisionThreadPool.Enqueue(&Weave::Physics::PhysicsHandler::ProcessEntityCollisions, this, objects));
	}

	std::set<std::pair<ECS::EntityID, ECS::EntityID>> processedCollisions;

	for (std::future<std::vector<Weave::Physics::EntityCollision>>& future : futures)
	{
		for (EntityCollision collision : future.get())
		{
			if (processedCollisions.contains({ collision.lhs, collision.rhs })) continue;
			processedCollisions.insert({ collision.lhs, collision.rhs });

			if (!world.HasComponent<Colliding>(collision.lhs)) world.AddComponent<Colliding>(collision.lhs);
			Colliding* component = world.GetComponent<Colliding>(collision.lhs);

			std::map<ECS::EntityID, CollisionData>::iterator iter = component->collisions.find(collision.rhs);

			if (iter == component->collisions.end())
			{
				component->collisions.emplace(collision.rhs, collision);
				continue;
			}

			CollisionData* collisionData = &((*iter).second);

			collisionData->state = CollisionState::Continued;
			collisionData->normal = collision.normal;
			collisionData->depth = collision.depth;
			collisionData->otherCollider = collision.otherCollider;
		}
	}
}

void Weave::Physics::PhysicsHandler::ProcessPhysics(ECS::World& world)
{
	const float scaledGravity = gravity * (float)Time::DeltaTime;

	for (auto [entity, transform, rigidbody] : world.GetView<Transform, Rigidbody>())
	{
		rigidbody.acceleration.y -= scaledGravity;
		rigidbody.velocity *= 1.0f / (1.0f + rigidbody.material.drag * (float)Time::DeltaTime);

		rigidbody.velocity += rigidbody.acceleration * (float)Time::DeltaTime;
		transform.position += rigidbody.velocity * (float)Time::DeltaTime;

		rigidbody.acceleration = { 0.0f, 0.0f };
	}
}

void Weave::Physics::PhysicsHandler::HandleCollisions(ECS::World& world)
{
	for (auto [entity, colliding, transform, rigidbody] : world.GetView<Colliding, Transform, Rigidbody>())
	{
		for (auto& [otherEntity, collisionData] : colliding.collisions)
		{
			if (entity >= otherEntity) continue; // Ensures each pair is processed only once.
			if (collisionData.state == CollisionState::Ended) continue; // Ensures ending collisions are not processed.

			Transform* otherTransform = world.GetComponent<Transform>(otherEntity);
			Rigidbody* otherRigidbody = world.GetComponent<Rigidbody>(otherEntity);

			if (!otherTransform || !otherRigidbody) continue; // Other is missing either a transform or a rigidbody, no response required.

			const float correctionFactor = 0.65f;
			const float slop = 0.01f;

			if (collisionData.depth < slop) continue;

			Mathematics::Vector2<float> correction = collisionData.normal * collisionData.depth * correctionFactor;

			float totalMass = rigidbody.mass + otherRigidbody->mass;

			if (totalMass > 0.0f)
			{
				transform.position += correction * (otherRigidbody->mass / totalMass);
				otherTransform->position -= correction * (rigidbody.mass / totalMass);
			}
			else
			{
				transform.position += correction * 0.5f;
				otherTransform->position -= correction * 0.5f;
			}

			if (rigidbody.mass == 0.0f || otherRigidbody->mass == 0.0f) continue; // Mass of at least one object is 0, no impulse necessary.

			Mathematics::Vector2<float> relativeVelocity = rigidbody.velocity - otherRigidbody->velocity;
			float velocityAlongNormal = Mathematics::Vector2<float>::Dot(relativeVelocity, collisionData.normal);

			if (velocityAlongNormal > 0.0f) continue; // Only resolve if objects are moving towards each other

			float restitution = std::min(rigidbody.material.bounciness, otherRigidbody->material.bounciness);
			float impulseScalar = -(1.0f + restitution) * velocityAlongNormal;

			float invMass1 = rigidbody.mass > 0.0f ? 1.0f / rigidbody.mass : 0.0f;
			float invMass2 = otherRigidbody->mass > 0.0f ? 1.0f / otherRigidbody->mass : 0.0f;
			impulseScalar /= invMass1 + invMass2;

			Mathematics::Vector2<float> impulse = collisionData.normal * impulseScalar;

			rigidbody.acceleration += impulse * invMass1;
			otherRigidbody->acceleration -= impulse * invMass2;

			Mathematics::Vector2<float> tangent = relativeVelocity - collisionData.normal * velocityAlongNormal;

			if (tangent.GetSquaredMagnitude() == 0.0f) continue;

			tangent = tangent.GetUnitVector();

			float friction = std::min(rigidbody.material.friction, otherRigidbody->material.friction);
			float frictionScalar = -Mathematics::Vector2<float>::Dot(relativeVelocity, tangent);
			frictionScalar /= invMass1 + invMass2;
			frictionScalar *= friction;

			Mathematics::Vector2<float> frictionImpulse = tangent * frictionScalar;

			rigidbody.acceleration += frictionImpulse * invMass1;
			otherRigidbody->acceleration -= frictionImpulse * invMass2;
		}
	}
}

void Weave::Physics::PhysicsHandler::HandlePhysics(Weave::ECS::World& world)
{
	UpdateCollisions(world);
	ProcessPhysics(world);
	CheckCollisions(world);
	HandleCollisions(world);
}

std::vector<Weave::Physics::EntityCollision> Weave::Physics::PhysicsHandler::ProcessEntityCollisions(std::vector<std::pair<Weave::ECS::EntityID, Weave::Physics::Collider>> entities)
{
	std::vector<EntityCollision> collisions;

	for (int i = 0; i < entities.size(); ++i)
	{
		for (int j = 0; j < entities.size(); ++j)
		{
			if (i == j) continue;

			std::optional<Collision> collision = Collides(entities[i].second, entities[j].second);
			if (collision)
			{
				collisions.push_back({
					collision.value().normal,
					collision.value().depth,
					entities[j].second,
					CollisionState::Started,
					entities[i].first,
					entities[j].first
					});
			}
		}
	}

	return collisions;
}
