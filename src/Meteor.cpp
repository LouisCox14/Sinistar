#pragma once
#include "Meteor.h"

Sinistar::MeteorManager::MeteorManager(Weave::GameEngine& engine)
{
	std::shared_ptr<sf::Texture> meteorTexture = engine.GetRenderer().GetTexture("spritesheet_meteors_30x30.png");
	meteorSpritesheet = Weave::Graphics::SpriteSheet(meteorTexture, Weave::Mathematics::Vector2<uint16_t>(30, 30));

	std::shared_ptr<sf::Texture> particleTexture = engine.GetRenderer().GetTexture("spritesheet_meteor_fragments_11x11.png");
	particleSpritesheet = Weave::Graphics::SpriteSheet(particleTexture, Weave::Mathematics::Vector2<uint16_t>(11, 11));
}

void Sinistar::MeteorManager::ResolveMeteorsHit(Weave::ECS::World& world)
{
	std::vector<Weave::ECS::EntityID> deletionQueue;

	for (auto [entity, mTag, hTag] : world.GetView<MeteorTag, KillTag>())
	{
		SinicrystalHolder* holder = world.TryGetComponent<SinicrystalHolder>(entity);

		if (holder && holder->sinicrystals > 0) continue;

		deletionQueue.push_back(entity);
	}

	Weave::Physics::Rigidbody particleRigidbody;
	particleRigidbody.collider.shape = Weave::Shapes::Circle(Weave::Mathematics::Vector2(0, 0), 0.4f);
	particleRigidbody.collider.layers = Weave::Physics::CollisionLayer::None;
	particleRigidbody.collider.layerMask = Weave::Physics::CollisionLayer::None;
	particleRigidbody.material.drag = 0.15f;
	particleRigidbody.mass = 0.5f;

	for (Weave::ECS::EntityID entity : deletionQueue)
	{
		Weave::Transform particleTransform;
		particleTransform.position = world.TryGetComponent<Weave::Transform>(entity)->position;

		for (int i = 0; i < Weave::Random::GenerateRandomInBounds(1, 4); i++)
		{
			float direction = Weave::Random::GenerateRandomInBounds(0.0f, 360.0f);
			particleRigidbody.acceleration = Weave::Mathematics::AngleToVector(direction) * 200;
			particleTransform.rotation = direction;

			Weave::ECS::EntityID particle = world.CreateEntity();
			world.AddComponents<Weave::Transform, Weave::Physics::Rigidbody, Weave::Graphics::Sprite, Weave::EntityTimeout>(
				particle,
				particleTransform,
				particleRigidbody,
				particleSpritesheet.GetSprite(Weave::Random::GenerateRandomInBounds(0, 6)),
				{ 4.0f }
			);
		}

		world.DeleteEntity(entity);
	}
}

Weave::ECS::EntityID Sinistar::MeteorManager::CreateMeteor(Weave::GameEngine& engine, Weave::Mathematics::Vector2<float> startPosition)
{
	Weave::Physics::Rigidbody meteorRigidbody;
	meteorRigidbody.collider.shape = Weave::Shapes::Circle(Weave::Mathematics::Vector2(0, 0), 0.4f);
	meteorRigidbody.collider.layers = Sinistar::CollisionLayers::Meteors;
	meteorRigidbody.collider.layerMask = Sinistar::CollisionLayers::Player | Sinistar::CollisionLayers::Drones | Sinistar::CollisionLayers::Meteors | Sinistar::CollisionLayers::Projectiles;
	meteorRigidbody.material.drag = 0.15f;
	meteorRigidbody.material.bounciness = 80.0f;
	meteorRigidbody.mass = 75.0f;

	Weave::Transform meteorTransform;
	meteorTransform.position = startPosition;

	Weave::ECS::EntityID meteor = engine.GetWorld().CreateEntity();
	engine.GetWorld().AddComponents<MeteorTag, Weave::Transform, Weave::Graphics::Sprite, Weave::Physics::Rigidbody, SinicrystalHolder, Health, MinimapEntity>(
		meteor,
		{ },
		meteorTransform,
		meteorSpritesheet.GetSprite(Weave::Random::GenerateRandomInBounds(0, 4)),
		meteorRigidbody,
		{ 1 },
		{ 6 },
		{ sf::Color(128, 128, 128) }
	);

	return meteor;
}
