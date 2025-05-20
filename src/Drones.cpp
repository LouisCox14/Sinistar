#pragma once
#include "Drones.h"

Sinistar::DroneManager::DroneManager(Weave::Engine& engine)
{
	std::shared_ptr<sf::Texture> workerTexture = engine.GetRenderer().GetTexture("spritesheet_worker_10x10.png");
	workerSpritesheet = Weave::Graphics::SpriteSheet(workerTexture, Weave::Mathematics::Vector2<uint16_t>(2, 2));

	std::shared_ptr<sf::Texture> warriorTexture = engine.GetRenderer().GetTexture("spritesheet_warrior_15x15.png");
	warriorSpritesheet = Weave::Graphics::SpriteSheet(warriorTexture, Weave::Mathematics::Vector2<uint16_t>(2, 2));
}
