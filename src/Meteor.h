#pragma once
#include "Engine/Engine.h"
#include "Engine/Physics.h"
#include "Engine/Random.h"
#include "Engine/EntityTimeout.h"
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
		MeteorManager(Weave::Engine& engine);

		void ResolveMeteorsHit(Weave::ECS::World& world);
		Weave::ECS::EntityID CreateMeteor(Weave::Engine& engine, Weave::Mathematics::Vector2<float> startPosition);
	};
}