#pragma once
#include "ShipMovement.h"

void Sinistar::ApplyShipAcceleration(Weave::ECS::World& world)
{
	for (auto [entity, shipMovement, rigidbody] : world.GetView<ShipMovement, Weave::Physics::Rigidbody>())
	{
		rigidbody.acceleration += shipMovement.direction * shipMovement.speed;
	}
}