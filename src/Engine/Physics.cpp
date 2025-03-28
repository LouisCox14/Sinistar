#pragma once
#include "Physics.h"

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

	QuadTree quadtree({ {-10.0f, -10.0f}, {10.0f, 10.0f} }, 5, 5);
	quadtree.Reserve(view.GetEntityCount());

	for (auto [entity, transform, rigidbody] : world.GetView<Transform, Rigidbody>())
	{
		quadtree.Insert(entity, Weave::Physics::TranslateCollider(rigidbody.collider, transform.position));
	}

	Weave::Utilities::ThreadPool collisionThreadPool(8);
	std::vector<std::future<std::vector<Weave::Physics::Collision>>> futures;

	for (Weave::QuadTree::NodeID nodeID : quadtree.GetAllLeafNodes())
	{
		std::vector<std::pair<ECS::EntityID, Physics::Collider>> objects = quadtree.GetLeafObjects(nodeID);

		if (objects.size() == 0) continue;

		futures.push_back(collisionThreadPool.Enqueue(&Weave::Physics::PhysicsHandler::ProcessEntityCollisions, this, objects));
	}

	std::set<std::pair<ECS::EntityID, ECS::EntityID>> processedCollisions;

	for (std::future<std::vector<Weave::Physics::Collision>>& future : futures)
	{
		for (Collision collision : future.get())
		{
			if (processedCollisions.contains({ collision.lhs, collision.rhs })) continue;
			processedCollisions.insert({ collision.lhs, collision.rhs });

			if (!world.HasComponent<Colliding>(collision.lhs)) world.AddComponent<Colliding>(collision.lhs);
			Colliding* component = world.GetComponent<Colliding>(collision.lhs);

			std::map<ECS::EntityID, CollisionData>::iterator iter = component->collisions.find(collision.rhs);

			if (iter == component->collisions.end())
			{
				component->collisions.emplace(collision.rhs, collision.data);
				continue;
			}

			CollisionData* collisionData = &((*iter).second);

			collisionData->state = CollisionState::Continued;
			collisionData->normal = collision.data.normal;
		}
	}
}

Weave::Physics::PhysicsHandler::PhysicsHandler(float _gravity) : gravity(_gravity) { }

void Weave::Physics::PhysicsHandler::HandlePhysics(Weave::ECS::World& world)
{
	UpdateCollisions(world);
	CheckCollisions(world);
}

std::vector<Weave::Physics::Collision> Weave::Physics::PhysicsHandler::ProcessEntityCollisions(std::vector<std::pair<Weave::ECS::EntityID, Weave::Physics::Collider>> entities)
{
	std::vector<Collision> collisions;

	for (int i = 0; i < entities.size(); ++i)
	{
		for (int j = 0; j < entities.size(); ++j)
		{
			if (i == j) continue;

			if (Collides(entities[i].second, entities[j].second))
			{
				collisions.push_back({
					entities[i].first,
					entities[j].first
					});
			}
		}
	}

	return collisions;
}
