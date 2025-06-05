#pragma once
#include "Mathematics.h"

namespace Weave
{
	namespace Shapes
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
	}
}