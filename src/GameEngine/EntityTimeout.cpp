#pragma once
#include "EntityTimeout.h"

void Weave::UpdateEntityTimeout(ECS::EntityID entity, EntityTimeout& timeoutComponent, ECS::CommandBuffer& cmd)
{
	timeoutComponent.time -= Weave::Time::DeltaTime;
	if (timeoutComponent.time > 0.0f) return;

	cmd.AddCommand([](Weave::ECS::World& world, Weave::ECS::EntityID entity) 
		{ 
			world.DeleteEntity(entity); 
		}, entity);
}