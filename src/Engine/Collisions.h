#pragma once
#include "Shapes.h"
#include "Mathematics.h"
#include <variant>

namespace Weave
{
	namespace Physics
	{
		struct Collider
		{
			std::variant<Shapes::Rectangle, Shapes::Circle, Shapes::Line> shape;
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