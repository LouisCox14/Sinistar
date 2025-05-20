#pragma once
#include "EntityTimeout.h"

void Weave::UpdateEntityTimeout(Weave::ECS::World& world)
{
	std::vector<Weave::ECS::EntityID> deletionQueue;

	for (auto [entity, timeoutComponent] : world.GetView<EntityTimeout>())
	{
		timeoutComponent.time -= Weave::Time::DeltaTime;
		if (timeoutComponent.time <= 0.0f) deletionQueue.push_back(entity);
	}

	for (Weave::ECS::EntityID entity : deletionQueue)
	{
		world.DeleteEntity(entity);
	}
}