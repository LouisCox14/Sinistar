#pragma once
#include "Mathematics.h"
#include "Transform.h"
#include "ECS.h"
#include "Collisions.h"
#include "Quadtree.h"
#include "ThreadPool.h"
#include "Time.h"
#include <vector>
#include <utility>
#include <set>
#include <queue>
#include <map>

namespace Weave
{
	namespace Physics
	{
        struct EntityCollision;

        class PhysicsHandler
        {
        private:
            void UpdateCollisions(ECS::World& world);
            void CheckCollisions(ECS::World& world);
            void ProcessPhysics(ECS::World& world);
            void HandleCollisions(ECS::World& world);

        public:
            float gravity = 0.0f;

            void HandlePhysics(ECS::World& world);
            std::vector<EntityCollision> ProcessEntityCollisions(std::vector<std::pair<ECS::EntityID, Physics::Collider>> entities);
        };

        enum CollisionState
        {
            Started,
            Continued,
            Ended
        };

        struct CollisionData : Collision
        {
            Collider otherCollider;
            CollisionState state;
        };

        struct EntityCollision : CollisionData
        {
            ECS::EntityID lhs;
            ECS::EntityID rhs;
        };

        struct PhysicsMaterial
        {
            float friction = 0.0f;
            float bounciness = 0.0f;
            float drag = 0.0f;
        };

		struct Rigidbody
		{
			Mathematics::Vector2<float> velocity;
            Mathematics::Vector2<float> acceleration;

            float mass = 0.0f;

            Collider collider;
            PhysicsMaterial material;
		};

        struct Colliding
        {
            std::map<ECS::EntityID, CollisionData> collisions;
        };
	}
}