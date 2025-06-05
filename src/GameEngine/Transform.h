#pragma once
#include "Mathematics.h"

namespace Weave
{
	struct Transform
	{
		Mathematics::Vector2<float> position = { 0, 0 };
		Mathematics::Vector2<float> scale = { 1, 1 };
		float rotation = 0;
	};
}
