#pragma once
#include "GameEngine/GameEngine.h"
#include "GameEngine/Physics.h"
#include "Health.h"
#include "CollisionLayers.h"

namespace Sinistar
{
	struct SinibombTag { };
	struct SinicrystalTag { };

	struct SinicrystalHolder
	{
		uint8_t sinicrystals;
	};

	class SinibombManager
	{
	private:
		Weave::Graphics::SpriteSheet sinibombSpritesheet;
		Weave::Graphics::SpriteSheet sinicrystalSpritesheet;

		Weave::ECS::EntityID CreateSinicrystal(Weave::ECS::World& world, Weave::Mathematics::Vector2<float> position);
		Weave::ECS::EntityID CreateSinibomb(Weave::ECS::World& world, Weave::Mathematics::Vector2<float> position);

	public:
		SinibombManager(Weave::GameEngine& engine);

		void UpdateSinicrystalHolders(Weave::ECS::World& world);
		void UpdateSinicrystals(Weave::ECS::World& world);
	};
}