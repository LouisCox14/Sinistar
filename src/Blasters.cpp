#pragma once
#include "Blasters.h"

void Sinistar::UpdateBlasters(Weave::ECS::World& world)
{
	std::vector<std::tuple<Weave::Mathematics::Vector2<float>, Weave::Mathematics::Vector2<float>, Sinistar::ProjectileTemplate>> spawnInfo;

	for (auto [entity, transform, blaster] : world.GetView<Weave::Transform, Blasters>())
	{
		blaster.timeSinceLastShot += Weave::Time::DeltaTime;

		if (blaster.isFiring && blaster.timeSinceLastShot >= blaster.cooldown)
		{
			blaster.timeSinceLastShot = 0.0f;
			spawnInfo.push_back({ transform.position, blaster.direction, blaster.projectileTemplate });
		}
	}

	for (auto [position, direction, projectileTemplate] : spawnInfo)
	{
		CreateProjectile(world, projectileTemplate, position, direction * 500.0f);
	}
}