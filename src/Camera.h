#pragma once
#include "Engine/Engine.h"
#include "Player.h"

namespace Sinistar
{
	class CameraManager
	{
	private:
		Weave::Graphics::Renderer* renderer;

	public:
		CameraManager(Weave::Engine& engine);

		void UpdateCamera(Weave::ECS::World& world);
	};
}