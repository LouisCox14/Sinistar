#include "Engine/Engine.h"
#include <iostream>

int main()
{
	Weave::Engine engine = Weave::Engine("Sinistar");

	std::shared_ptr<sf::Texture> sinistarSpritesheet = engine.GetRenderer().GetTexture("spritesheet_sinistar.png");

	std::cout << "Loaded a " << sinistarSpritesheet->getSize().x << "x" << sinistarSpritesheet->getSize().y << " texture.\n";

	Weave::ECS::EntityID sinistar = engine.GetWorld().CreateEntity();

	Weave::Transform transform;

	Weave::Graphics::Sprite sprite{ sinistarSpritesheet, Weave::Mathematics::Vector2<uint16_t>(0, 0), Weave::Mathematics::Vector2<uint16_t>(50, 50) };

	engine.GetWorld().AddComponents<Weave::Transform>(sinistar, transform);
	engine.GetWorld().AddComponents<Weave::Graphics::Sprite>(sinistar, sprite);

	engine.Run();

	return 0;
}