#pragma once
#include "GameEngine/GameEngine.h"
#include "CollisionLayers.h"

namespace Sinistar
{
	struct DroneTag { };
	struct WorkerTag { };
	struct WarriorTag { };

	class DroneManager
	{
	private:
		Weave::Graphics::SpriteSheet workerSpritesheet;
		Weave::Graphics::SpriteSheet warriorSpritesheet;

	public:
		DroneManager(Weave::GameEngine& engine);
	};
}