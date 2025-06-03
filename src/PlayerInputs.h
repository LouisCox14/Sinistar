#pragma once
#include "Engine/Mathematics.h"
#include "Engine/Engine.h"
#include "Engine/Events.h"
#include "Engine/Transform.h"
#include "Player.h"
#include "ShipMovement.h"
#include "Blasters.h"

namespace Sinistar
{
	class PlayerInputs
	{
	private:
		Weave::Engine& engine;

		void HandleMouseMoved(Weave::Mathematics::Vector2<float> newMousePos);

		void HandleRightClickStarted();
		void HandleRightClickEnded();

		void HandleLeftClickStarted();
		void HandleLeftClickEnded();

		Weave::Mathematics::Vector2<float> targetPosition;

		void UpdatePlayerMovement(Weave::ECS::World& world);
		void UpdatePlayerRotation(Weave::ECS::World& world);

		void UpdateBlasterTargetting(Weave::ECS::World& world);

	public:
		PlayerInputs(Weave::Engine& engine);
	};
}