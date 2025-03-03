#include "Engine/Engine.h"
#include "PlayerInputs.h"
#include "Player.h"
#include <iostream>

#include "Engine/Physics.h"

int main()
{
	Weave::Engine engine = Weave::Engine("Sinistar");

	std::shared_ptr<sf::Texture> sinistarTexture = engine.GetRenderer().GetTexture("spritesheet_sinistar.png");
	Weave::Graphics::SpriteSheet sinistarSpritesheet(sinistarTexture, Weave::Mathematics::Vector2<uint16_t>(53, 53));

	std::shared_ptr<sf::Texture> playerTexture = engine.GetRenderer().GetTexture("spritesheet_player.png");
	Weave::Graphics::SpriteSheet playerSpritesheet(playerTexture, Weave::Mathematics::Vector2<uint16_t>(11, 11));

	Weave::ECS::EntityID sinistar = engine.GetWorld().CreateEntity();
	engine.GetWorld().AddComponents<Weave::Transform, Weave::Graphics::Sprite>(sinistar, { }, sinistarSpritesheet.GetSprite(26));

	Weave::ECS::EntityID player = engine.GetWorld().CreateEntity();
	engine.GetWorld().AddComponents<PlayerTag, Weave::Transform, Weave::Graphics::Sprite>(player, { }, { Weave::Mathematics::Vector2<float>(2, 1) }, playerSpritesheet.GetSprite(1));

	PlayerInputs playerInputs(engine);

	engine.Run();

	return 0;
}