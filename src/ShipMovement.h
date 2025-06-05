#pragma once
#include "GameEngine/Mathematics.h"
#include "ECS.h"
#include "GameEngine/Physics.h"

namespace Sinistar
{
	struct ShipMovement
	{
		Weave::Mathematics::Vector2<float> direction;
		float speed = 0.0f;
	};

	void ApplyShipAcceleration(Weave::ECS::EntityID entity, ShipMovement& shipMovement, Weave::Physics::Rigidbody& rigidbody);
}