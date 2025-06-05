#pragma once
#include "Projectile.h"
#include "GameEngine/GameEngine.h"
#include "GameEngine/Time.h"

namespace Sinistar
{
	struct Blasters
	{
		bool isFiring = false;

		float cooldown = 0.0f;
		float timeSinceLastShot = 0.0f;

		Weave::Mathematics::Vector2<float> direction = { 0.0f, 0.0f };

		Sinistar::ProjectileTemplate projectileTemplate;
	};

	void UpdateBlasters(Weave::ECS::World& world);
}