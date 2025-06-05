#pragma once
#include "GameEngine/GameEngine.h"
#include "Player.h"

namespace Sinistar
{
	class CameraManager
	{
	private:
		Weave::Graphics::Renderer* renderer;

	public:
		CameraManager(Weave::GameEngine& engine);

		void UpdateCamera(Weave::ECS::World& world);
	};
}