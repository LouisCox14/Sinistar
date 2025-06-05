#pragma once
#include "GameEngine/GameEngine.h"
#include "GameEngine/Physics.h"
#include "GameEngine/Random.h"
#include "GameEngine/EntityTimeout.h"
#include "CollisionLayers.h"
#include "Health.h"
#include "Sinibomb.h"
#include "Minimap.h"

namespace Sinistar
{
	struct MeteorTag { };

	class MeteorManager
	{
	private:
		Weave::Graphics::SpriteSheet meteorSpritesheet;
		Weave::Graphics::SpriteSheet particleSpritesheet;

	public:
		MeteorManager(Weave::GameEngine& engine);

		void ResolveMeteorsHit(Weave::ECS::World& world);
		Weave::ECS::EntityID CreateMeteor(Weave::GameEngine& engine, Weave::Mathematics::Vector2<float> startPosition);
	};
}