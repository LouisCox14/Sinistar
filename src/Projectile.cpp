#pragma once
#include "Projectile.h"

Weave::ECS::EntityID Sinistar::CreateProjectile(Weave::ECS::World& world, ProjectileTemplate projectileTemplate, Weave::Mathematics::Vector2<float> startPos, Weave::Mathematics::Vector2<float> initialForce)
{
	Weave::Physics::Rigidbody projectileRigidbody;
	projectileRigidbody.mass = 10.0f;
	projectileRigidbody.acceleration = initialForce;

	projectileRigidbody.collider.shape = Weave::Shapes::Circle(Weave::Mathematics::Vector2(0, 0), 0.05f);
	projectileRigidbody.collider.layerMask = projectileTemplate.targetLayers;
	projectileRigidbody.collider.layers = CollisionLayers::Projectiles;

	Weave::Transform projectileTransform;
	projectileTransform.position = startPos;
	projectileTransform.rotation = initialForce.GetUnitVector().GetAngle();

	Weave::ECS::EntityID projectile = world.CreateEntity();

	world.AddComponents<ProjectileTag, Weave::EntityTimeout, Weave::Graphics::Sprite, Weave::Transform, Weave::Physics::Rigidbody>(
		projectile,
		{ },
		{ 3.0f },
		projectileTemplate.sprite,
		projectileTransform,
		projectileRigidbody
	);

	return projectile;
}

void Sinistar::UpdateProjectiles(Weave::ECS::World& world)
{
	std::vector<Weave::ECS::EntityID> entitiesHit;
	std::vector<Weave::ECS::EntityID> projectilesHit;

	for (auto [entity, colliding, tag] : world.GetView<Weave::Physics::Colliding, Sinistar::ProjectileTag>())
	{
		projectilesHit.push_back(entity);

		for (auto [otherEntity, collisionData] : colliding.collisions)
		{
			entitiesHit.push_back(otherEntity);
		}
	}

	for (Weave::ECS::EntityID entity : entitiesHit)
	{
		Health* entityHealth = world.TryGetComponent<Health>(entity);

		if (!entityHealth) continue;

		entityHealth->health--;

		if (entityHealth->health > 0) continue;

		world.RemoveComponent<Health>(entity);
		world.AddComponent<KillTag>(entity);
	}

	for (Weave::ECS::EntityID projectile : projectilesHit)
	{
		world.DeleteEntity(projectile);
	}
}