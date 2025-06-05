#pragma once
#include "ShipMovement.h"

void Sinistar::ApplyShipAcceleration(Weave::ECS::EntityID entity, ShipMovement& shipMovement, Weave::Physics::Rigidbody& rigidbody)
{
	rigidbody.acceleration += shipMovement.direction * shipMovement.speed;
}