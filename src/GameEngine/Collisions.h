#pragma once
#include "Shapes.h"
#include "Mathematics.h"
#include <variant>
#include <bitset>
#include <optional>

namespace Weave
{
	namespace Physics
	{
        enum CollisionLayer
        {
            Default = 1,
            All = 16777215, // The maximum unsigned int for 24 bits, all 1s.
            None = 0,
            Layer1 = 1 << 1,
            Layer2 = 1 << 2,
            Layer3 = 1 << 3,
            Layer4 = 1 << 4,
            Layer5 = 1 << 5,
            Layer6 = 1 << 6,
            Layer7 = 1 << 7,
            Layer8 = 1 << 8,
            Layer9 = 1 << 9,
            Layer10 = 1 << 10,
            Layer11 = 1 << 11,
            Layer12 = 1 << 12,
            Layer13 = 1 << 13,
            Layer14 = 1 << 14,
            Layer15 = 1 << 15,
            Layer16 = 1 << 16,
            Layer17 = 1 << 17,
            Layer18 = 1 << 18,
            Layer19 = 1 << 19,
            Layer20 = 1 << 20,
            Layer21 = 1 << 21,
            Layer22 = 1 << 22,
            Layer23 = 1 << 23
        };

        using LayerMask = std::bitset<24>;

		struct Collider
		{
			std::variant<Shapes::Rectangle, Shapes::Circle, Shapes::Line> shape;
            LayerMask layerMask = CollisionLayer::All;
            LayerMask layers = CollisionLayer::Default;
		};

        struct Collision
        {
            Mathematics::Vector2<float> normal;
            float depth;
        };

        // TODO: Make all collision checks take const references as arguments.

        std::optional<Collision> Collides(Collider lhs, Collider rhs);

        std::optional<Collision> Collides(Shapes::Rectangle lhs, Shapes::Rectangle rhs);
        std::optional<Collision> Collides(Shapes::Rectangle lhs, Shapes::Circle rhs);
        std::optional<Collision> Collides(Shapes::Rectangle lhs, Shapes::Line rhs);
        std::optional<Collision> Collides(Shapes::Circle lhs, Shapes::Circle rhs);
        std::optional<Collision> Collides(Shapes::Circle lhs, Shapes::Line rhs);
        std::optional<Collision> Collides(Shapes::Line lhs, Shapes::Line rhs);

        // Overriding for point checks.

        std::optional<Collision> Collides(Mathematics::Vector2<float> point, Shapes::Rectangle collider);
        std::optional<Collision> Collides(Mathematics::Vector2<float> point, Shapes::Circle collider);

        // Overriding for alternate parameter orders.

        std::optional<Collision> Collides(Shapes::Circle lhs, Shapes::Rectangle rhs);
        std::optional<Collision> Collides(Shapes::Line lhs, Shapes::Rectangle rhs);
        std::optional<Collision> Collides(Shapes::Line lhs, Shapes::Circle rhs);

        Collider TranslateCollider(Collider collider, Mathematics::Vector2<float> offset);

        Collider TranslateCollider(Shapes::Rectangle collider, Mathematics::Vector2<float> offset);
        Collider TranslateCollider(Shapes::Circle collider, Mathematics::Vector2<float> offset);
        Collider TranslateCollider(Shapes::Line collider, Mathematics::Vector2<float> offset);
	}
}