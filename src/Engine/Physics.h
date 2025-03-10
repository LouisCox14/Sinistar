#pragma once
#include <variant>
#include "Mathematics.h"
#include "Transform.h"
#include "ECS.h"
#include "Shapes.h"

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
        };

        struct Collider
        {
            std::variant<Shapes::Rectangle, Shapes::Circle, Shapes::Line> shape;
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

        bool Collides(Collider lhs, Collider rhs);

        bool Collides(Shapes::Rectangle lhs, Shapes::Rectangle rhs);
        bool Collides(Shapes::Rectangle lhs, Shapes::Circle rhs);
        bool Collides(Shapes::Rectangle lhs, Shapes::Line rhs);
        bool Collides(Shapes::Circle lhs, Shapes::Circle rhs);
        bool Collides(Shapes::Circle lhs, Shapes::Line rhs);
        bool Collides(Shapes::Line lhs, Shapes::Line rhs);

        // Overriding for point checks.

        bool Collides(Mathematics::Vector2<float> point, Shapes::Rectangle collider);
        bool Collides(Mathematics::Vector2<float> point, Shapes::Circle collider);

        // Overriding for alternate parameter orders.

        bool Collides(Shapes::Circle lhs, Shapes::Rectangle rhs);
        bool Collides(Shapes::Line lhs, Shapes::Rectangle rhs);
        bool Collides(Shapes::Line lhs, Shapes::Circle rhs);

        Collider TranslateCollider(Collider collider, Mathematics::Vector2<float> offset);

        Collider TranslateCollider(Shapes::Rectangle collider, Mathematics::Vector2<float> offset);
        Collider TranslateCollider(Shapes::Circle collider, Mathematics::Vector2<float> offset);
        Collider TranslateCollider(Shapes::Line collider, Mathematics::Vector2<float> offset);
	}
}