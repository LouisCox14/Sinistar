#pragma once
#include "Engine/Engine.h"

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
		MinimapManager(Weave::Engine& engine);
		~MinimapManager();

		void DrawMinimap(Weave::ECS::World& world);
	};
}