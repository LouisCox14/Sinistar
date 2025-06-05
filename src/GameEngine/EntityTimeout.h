#pragma once
#include "Engine.h"
#include "Time.h"

namespace Weave
{
	struct EntityTimeout
	{
		float time = 0.0f;
	};

	void UpdateEntityTimeout(ECS::EntityID entity, EntityTimeout& timoutComponent, ECS::CommandBuffer& cmd);
}