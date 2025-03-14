#pragma once
#include "Physics.h"

Weave::Physics::PhysicsHandler::PhysicsHandler(float _gravity) : gravity(_gravity) { }

void Weave::Physics::PhysicsHandler::HandlePhysics(Weave::ECS::World& world)
{
	std::chrono::time_point startTime = std::chrono::high_resolution_clock::now();

	QuadTree quadtree({ {-10.0f, -10.0f}, {10.0f, 10.0f} }, 5, 5);
	quadtree.Reserve(10000);

	for (auto [entity, transform, rigidbody] : world.GetView<Transform, Rigidbody>())
	{
		quadtree.Insert(entity, Weave::Physics::TranslateCollider(rigidbody.collider, transform.position));
	}

	Weave::Utilities::ThreadPool collisionThreadPool(8);
	std::vector<std::future<void>> futures;

	std::cout << "Generated quad tree in " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count() << " milliseconds\n";
	startTime = std::chrono::high_resolution_clock::now();

	for (Weave::QuadTree::NodeID nodeID : quadtree.GetAllLeafNodes())
	{
		std::vector<std::pair<ECS::EntityID, Physics::Collider>> objects  = quadtree.GetLeafObjects(nodeID);

		if (objects.size() == 0) continue;

		futures.push_back(collisionThreadPool.Enqueue(&Weave::Physics::PhysicsHandler::ProcessEntityCollisions, this, objects));
	}

	for (std::future<void>& future : futures)
	{
		future.get();
	}

	std::cout << "Queried collisions in " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count() << " milliseconds\n";
}

void Weave::Physics::PhysicsHandler::ProcessEntityCollisions(std::vector<std::pair<ECS::EntityID, Physics::Collider>> entities)
{
	for (int i = 0; i < entities.size(); ++i)
	{
		for (int j = i + 1; j < entities.size(); ++j)
		{
			if (Collides(entities[i].second, entities[j].second))
			{
				// TODO: Handle Collision.
			}
		}
	}
}
