#pragma once
#include "GameEngine/GameEngine.h"
#include "GameEngine/Physics.h"
#include "ShipMovement.h"
#include "CollisionLayers.h"
#include "Blasters.h"
#include "Sinibomb.h"
#include "Minimap.h"

namespace Sinistar
{
	struct PlayerTag { };

	Weave::ECS::EntityID CreatePlayer(Weave::GameEngine& engine);
}