#pragma once
#include "GameEngine/Mathematics.h"
#include "GameEngine/GameEngine.h"
#include "GameEngine/Events.h"
#include "GameEngine/Transform.h"
#include "Player.h"
#include "ShipMovement.h"
#include "Blasters.h"

namespace Sinistar
{
	class PlayerInputs
	{
	private:
		Weave::GameEngine& engine;

		void HandleMouseMoved(Weave::Mathematics::Vector2<float> newMousePos);

		void HandleRightClickStarted();
		void HandleRightClickEnded();

		void HandleLeftClickStarted();
		void HandleLeftClickEnded();

		Weave::Mathematics::Vector2<float> targetPosition;

		void UpdatePlayerMovement(Weave::ECS::World& world);
		void UpdatePlayerRotation(Weave::ECS::World& world);

		void UpdateBlasterTargetting(Weave::ECS::World& world);

		Weave::ECS::SystemID moveSystem;
		Weave::ECS::SystemID targetSystem;

	public:
		PlayerInputs(Weave::GameEngine& engine);
	};
}