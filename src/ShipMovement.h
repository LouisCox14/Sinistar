#pragma once
#include "Engine/Mathematics.h"
#include "ECS.h"
#include "Engine/Physics.h"

namespace Sinistar
{
	struct ShipMovement
	{
		Weave::Mathematics::Vector2<float> direction;
		float speed = 0.0f;
	};

	void ApplyShipAcceleration(Weave::ECS::World& world);
}