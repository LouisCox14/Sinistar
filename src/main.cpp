#pragma once
#include "Engine/Engine.h"
#include "PlayerInputs.h"
#include "Player.h"
#include "Meteor.h"
#include "Engine/Random.h"
#include "Engine/Physics.h"
#include "ShipMovement.h"
#include "Engine/EntityTimeout.h"
#include "Camera.h"
#include "Minimap.h"
#include <iostream>

int main()
{
	Weave::Engine engine = Weave::Engine("Sinistar");

	engine.GetWorld().GetSystemGroup(engine.lateUpdateGroup).Subscribe(&Weave::UpdateEntityTimeout);

	Weave::Physics::PhysicsHandler physicsHandler;

	engine.GetWorld().GetSystemGroup(engine.lateFixedUpdateGroup).Subscribe(physicsHandler, &Weave::Physics::PhysicsHandler::HandlePhysics);
	engine.GetWorld().GetSystemGroup(engine.fixedUpdateGroup).Subscribe(&Sinistar::ApplyShipAcceleration);

	engine.GetWorld().GetSystemGroup(engine.lateUpdateGroup).Subscribe(&Sinistar::UpdateBlasters);
	engine.GetWorld().GetSystemGroup(engine.lateFixedUpdateGroup).Subscribe(&Sinistar::UpdateProjectiles);

	Sinistar::MeteorManager meteorManager(engine);
	engine.GetWorld().GetSystemGroup(engine.lateFixedUpdateGroup).Subscribe(meteorManager, &Sinistar::MeteorManager::ResolveMeteorsHit);

	Sinistar::SinibombManager sinibombManager(engine);
	engine.GetWorld().GetSystemGroup(engine.updateGroup).Subscribe(sinibombManager, &Sinistar::SinibombManager::UpdateSinicrystalHolders);
	engine.GetWorld().GetSystemGroup(engine.lateFixedUpdateGroup).Subscribe(sinibombManager, &Sinistar::SinibombManager::UpdateSinicrystals);

	Sinistar::CameraManager cameraManager(engine);
	engine.GetWorld().GetSystemGroup(engine.lateUpdateGroup).Subscribe(cameraManager, &Sinistar::CameraManager::UpdateCamera);

	Sinistar::MinimapManager minimapManager(engine);
	engine.GetWorld().GetSystemGroup(engine.uiRenderGroup).Subscribe(minimapManager, &Sinistar::MinimapManager::DrawMinimap);

	Weave::ECS::EntityID player = Sinistar::CreatePlayer(engine);

	for (int i = 0; i < 300; i++)
	{
		meteorManager.CreateMeteor(engine, { Weave::Random::GenerateRandomInBounds(-100.0f, 100.0f), Weave::Random::GenerateRandomInBounds(-100.0f, 100.0f) });
	}

	Sinistar::PlayerInputs playerInputs(engine);

	engine.Run();

	return 0;
}