#pragma once
#include "Mathematics.h"
#include "Transform.h"
#include "ECS.h"
#include "Collisions.h"
#include "Quadtree.h"
#include "ThreadPool.h"
#include <vector>
#include <utility>
#include <set>
#include <queue>

namespace Weave
{
	namespace Physics
	{
        struct Collision;

        class PhysicsHandler
        {
        private:
            float gravity;

            void UpdateCollisions(ECS::World& world);
            void CheckCollisions(ECS::World& world);

        public:
            PhysicsHandler(float gravity);

            void HandlePhysics(ECS::World& world);
            std::vector<Collision> ProcessEntityCollisions(std::vector<std::pair<ECS::EntityID, Physics::Collider>> entities);
        };

        enum CollisionState
        {
            Started,
            Continued,
            Ended
        };

        struct CollisionData
        {
            Collider otherCollider;
            Mathematics::Vector2<float> normal;
            CollisionState state;
        };

        struct Collision
        {
            ECS::EntityID lhs;
            ECS::EntityID rhs;

            CollisionData data;
        };

        struct PhysicsMaterial
        {
            float friction;
            float bounciness;
            float drag;
        };

		struct Rigidbody
		{
			Mathematics::Vector2<float> velocity;
            Collider collider;
            PhysicsMaterial material;
		};

        struct Colliding
        {
            std::map<ECS::EntityID, CollisionData> collisions;
        };
	}
}