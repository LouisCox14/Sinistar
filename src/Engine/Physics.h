#pragma once
#include "Mathematics.h"
#include "Transform.h"
#include "ECS.h"
#include "Collisions.h"
#include "Quadtree.h"
#include "ThreadPool.h"

namespace Weave
{
	namespace Physics
	{
        class PhysicsHandler
        {
        private:
            float gravity;

        public:
            PhysicsHandler(float gravity);

            void HandlePhysics(ECS::World& world);
            void ProcessEntityCollisions(std::vector<std::pair<ECS::EntityID, Physics::Collider>> entities);
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
	}
}