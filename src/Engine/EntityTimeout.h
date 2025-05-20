#pragma once
#include "ECS.h"
#include "Time.h"

namespace Weave
{
	struct EntityTimeout
	{
		float time = 0.0f;
	};

	void UpdateEntityTimeout(Weave::ECS::World& world);
}