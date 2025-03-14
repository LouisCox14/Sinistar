#include "Engine/Engine.h"
#include "PlayerInputs.h"
#include "Player.h"
#include "Engine/Physics.h"
#include <iostream>
#include <random>

int main()
{
	Weave::Engine engine = Weave::Engine("Sinistar");

	std::shared_ptr<sf::Texture> sinistarTexture = engine.GetRenderer().GetTexture("spritesheet_sinistar.png");
	Weave::Graphics::SpriteSheet sinistarSpritesheet(sinistarTexture, Weave::Mathematics::Vector2<uint16_t>(53, 53));

	std::shared_ptr<sf::Texture> playerTexture = engine.GetRenderer().GetTexture("spritesheet_player.png");
	Weave::Graphics::SpriteSheet playerSpritesheet(playerTexture, Weave::Mathematics::Vector2<uint16_t>(11, 11));

	Weave::Physics::PhysicsHandler physicsHandler(0);
	engine.GetWorld().GetSystemGroup(engine.updateGroup).Subscribe(physicsHandler, &Weave::Physics::PhysicsHandler::HandlePhysics);

	Weave::Physics::Rigidbody playerRigidbody;
	playerRigidbody.collider.shape = Weave::Shapes::Circle(Weave::Mathematics::Vector2(0, 0), 0.25f);

	Weave::ECS::EntityID player = engine.GetWorld().CreateEntity();
	engine.GetWorld().AddComponents<PlayerTag, Weave::Transform, Weave::Graphics::Sprite, Weave::Physics::Rigidbody>(
		player,
		{ },
		{ },
		playerSpritesheet.GetSprite(1),
		playerRigidbody
	);

	Weave::Physics::Rigidbody testRigidbody;
	testRigidbody.collider.shape = Weave::Shapes::Circle(Weave::Mathematics::Vector2(0, 0), 0.25f);

	Weave::Transform testTransform;
	testTransform.scale = { 0.25f, 0.25f };

	std::default_random_engine gen;
	std::uniform_real_distribution<float> distribution(-10, 10);

	for (int i = 0; i < 10000; i++)
	{
		testTransform.position = { distribution(gen), distribution(gen) };

		Weave::ECS::EntityID testEntity = engine.GetWorld().CreateEntity();
		engine.GetWorld().AddComponents<Weave::Transform, Weave::Graphics::Sprite, Weave::Physics::Rigidbody>(
			testEntity,
			testTransform,
			sinistarSpritesheet.GetSprite(26),
			testRigidbody
		);
	}

	PlayerInputs playerInputs(engine);

	engine.Run();

	return 0;
}