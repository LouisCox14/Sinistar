#pragma once
#include "GameEngine/GameEngine.h"
#include "GameEngine/Physics.h"
#include "CollisionLayers.h"
#include "GameEngine/EntityTimeout.h"
#include "Health.h"

namespace Sinistar
{
	struct ProjectileTag { };

	struct ProjectileTemplate
	{
		Weave::Graphics::Sprite sprite;
		Weave::Physics::LayerMask targetLayers;
	};

	Weave::ECS::EntityID CreateProjectile(Weave::ECS::World& world, ProjectileTemplate projectileTemplate, Weave::Mathematics::Vector2<float> startPos, Weave::Mathematics::Vector2<float> initialForce);
	void UpdateProjectiles(Weave::ECS::World& world);
}