#pragma once
#include <variant>
#include "Mathematics.h"

namespace Weave
{
	namespace Physics
	{
        struct Rectangle 
        {
            Mathematics::Vector2<float> min;
            Mathematics::Vector2<float> max;
        };

        struct Circle 
        {
            Mathematics::Vector2<float> center;
            float radius;
        };

        struct Line 
        {
            Mathematics::Vector2<float> start;
            Mathematics::Vector2<float> end;
        };

        struct Collider
        {
            std::variant<Rectangle, Circle, Line> shape;
        };

		struct Rigidbody
		{
			Mathematics::Vector2<float> velocity;
            Collider collider;
		};

        bool Collides(Collider lhs, Collider rhs);

        bool Collides(Rectangle lhs, Rectangle rhs);
        bool Collides(Rectangle lhs, Circle rhs);
        bool Collides(Rectangle lhs, Line rhs);
        bool Collides(Circle lhs, Circle rhs);
        bool Collides(Circle lhs, Line rhs);
        bool Collides(Line lhs, Line rhs);

        // Overriding for alternate parameter orders.

        bool Collides(Circle lhs, Rectangle rhs);
        bool Collides(Line lhs, Rectangle rhs);
        bool Collides(Line lhs, Circle rhs);
	}
}