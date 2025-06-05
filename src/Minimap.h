#pragma once
#include "GameEngine/GameEngine.h"

namespace Sinistar
{
	struct MinimapEntity
	{
		sf::Color colour;
	};

	class MinimapManager
	{
	private:
		Weave::Graphics::Renderer* renderer;
		sf::Sprite* minimapBorder;

	public:
		MinimapManager(Weave::GameEngine& engine);
		~MinimapManager();

		void DrawMinimap(Weave::ECS::World& world);
	};
}