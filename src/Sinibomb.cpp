#pragma once
#include "Sinibomb.h"

Weave::ECS::EntityID Sinistar::SinibombManager::CreateSinicrystal(Weave::ECS::World& world, Weave::Mathematics::Vector2<float> position)
{
	Weave::Physics::Rigidbody sinicrystalRigidbody;
	sinicrystalRigidbody.collider.shape = Weave::Shapes::Circle(Weave::Mathematics::Vector2(0, 0), 0.15f);
	sinicrystalRigidbody.collider.layers = Sinistar::CollisionLayers::Sinicrystals;
	sinicrystalRigidbody.collider.layerMask = Sinistar::CollisionLayers::Drones | Sinistar::CollisionLayers::Player;

	Weave::Graphics::Animator sinicrystalAnimator;
	sinicrystalAnimator.spriteSheet = &sinicrystalSpritesheet;
	sinicrystalAnimator.frameTime = 0.15f;
	sinicrystalAnimator.frames = { 0, 1 };

	Weave::ECS::EntityID sinicrystal = world.CreateEntity();
	world.AddComponents<SinicrystalTag, Weave::Graphics::Sprite, Weave::Graphics::Animator, Weave::Transform, Weave::Physics::Rigidbody>(
		sinicrystal,
		{ },
		sinicrystalSpritesheet.GetSprite(0),
		sinicrystalAnimator,
		{ position },
		sinicrystalRigidbody
	);

	return sinicrystal;
}

Sinistar::SinibombManager::SinibombManager(Weave::Engine& engine)
{
	std::shared_ptr<sf::Texture> sinicrystalTexture = engine.GetRenderer().GetTexture("sinicrystal_2x2.png");
	sinicrystalSpritesheet = Weave::Graphics::SpriteSheet(sinicrystalTexture, Weave::Mathematics::Vector2<uint16_t>(2, 2));
}

void Sinistar::SinibombManager::UpdateSinicrystalHolders(Weave::ECS::World& world)
{
	for (auto [entity, transform, crystalHolder, tag] : world.GetView<Weave::Transform, SinicrystalHolder, KillTag>())
	{
		for (int i = 0; i < crystalHolder.sinicrystals; i++)
			CreateSinicrystal(world, transform.position);

		crystalHolder.sinicrystals = 0;
	}
}

void Sinistar::SinibombManager::UpdateSinicrystals(Weave::ECS::World& world)
{
	std::vector<Weave::ECS::EntityID> deletionQueue;

	for (auto [entity, colliding, tag] : world.GetView<Weave::Physics::Colliding, SinicrystalTag>())
	{
		for (std::map<Weave::ECS::EntityID, Weave::Physics::CollisionData>::iterator collision = colliding.collisions.begin(); collision != colliding.collisions.end(); collision++)
		{
			SinicrystalHolder* crystalHolder = world.GetComponent<SinicrystalHolder>(collision->first);

			if (!crystalHolder) return;

			crystalHolder->sinicrystals++;
			deletionQueue.push_back(entity);
			break;
		}
	}

	for (Weave::ECS::EntityID entity : deletionQueue)
	{
		world.DeleteEntity(entity);
	}
}
