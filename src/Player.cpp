#pragma once
#include "Player.h"

Weave::ECS::EntityID Sinistar::CreatePlayer(Weave::GameEngine& engine)
{
	std::shared_ptr<sf::Texture> playerTexture = engine.GetRenderer().GetTexture("spritesheet_player_11x11.png");
	Weave::Graphics::SpriteSheet playerSpritesheet(playerTexture, Weave::Mathematics::Vector2<uint16_t>(11, 11));

	std::shared_ptr<sf::Texture> laserTexture = engine.GetRenderer().GetTexture("laser_3x8.png");
	Weave::Graphics::SpriteSheet laserSpritesheet(laserTexture, Weave::Mathematics::Vector2<uint16_t>(3, 8));

	Weave::Physics::Rigidbody playerRigidbody;
	playerRigidbody.collider.shape = Weave::Shapes::Circle(Weave::Mathematics::Vector2(0, 0), 0.25f);
	playerRigidbody.collider.layers = Sinistar::CollisionLayers::Player;
	playerRigidbody.collider.layerMask = Sinistar::CollisionLayers::Meteors | Sinistar::CollisionLayers::Drones;
	playerRigidbody.material.drag = 0.85f;
	playerRigidbody.material.bounciness = 80.0f;
	playerRigidbody.mass = 3.5f;

	Blasters blasters;
	blasters.cooldown = 0.25f;
	blasters.projectileTemplate = ProjectileTemplate(laserSpritesheet.GetSprite(0), Sinistar::CollisionLayers::Drones | Sinistar::CollisionLayers::Sinistar | Sinistar::CollisionLayers::Meteors);

	Weave::ECS::EntityID player = engine.GetWorld().CreateEntity();
	engine.GetWorld().AddComponents<PlayerTag, ShipMovement, Blasters, Weave::Transform, Weave::Graphics::Sprite, Weave::Physics::Rigidbody, SinicrystalHolder, MinimapEntity>(
		player,
		{ },
		{ Weave::Mathematics::Vector2<float>(), 8.5f },
		blasters,
		{ },
		playerSpritesheet.GetSprite(0),
		playerRigidbody,
		{ },
		{ sf::Color::Yellow }
	);

	return player;
}