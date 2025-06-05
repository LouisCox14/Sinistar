#pragma once
#include "GameEngine/GameEngine.h"
#include "PlayerInputs.h"
#include "Player.h"
#include "Meteor.h"
#include "GameEngine/Random.h"
#include "GameEngine/Physics.h"
#include "ShipMovement.h"
#include "GameEngine/EntityTimeout.h"
#include "Camera.h"
#include "Minimap.h"
#include <iostream>

int main()
{
	Weave::GameEngine engine = Weave::GameEngine("Sinistar");

	engine.RegisterSystemThreaded<Weave::EntityTimeout>(engine.updateGroup, &Weave::UpdateEntityTimeout, 999.0f);

	Weave::Physics::PhysicsHandler physicsHandler;

	engine.RegisterSystem(engine.fixedUpdateGroup, [physicsHandler = &physicsHandler](Weave::ECS::World& world) { physicsHandler->HandlePhysics(world); }, 10.0f);
	engine.RegisterSystem<Sinistar::ShipMovement, Weave::Physics::Rigidbody>(engine.fixedUpdateGroup, &Sinistar::ApplyShipAcceleration);

	engine.RegisterSystem(engine.updateGroup, &Sinistar::UpdateBlasters, -1.0f);
	engine.RegisterSystem(engine.fixedUpdateGroup, &Sinistar::UpdateProjectiles, 11.0f);

	Sinistar::MeteorManager meteorManager(engine);
	engine.RegisterSystem(engine.fixedUpdateGroup, [meteorManager = &meteorManager](Weave::ECS::World& world) { meteorManager->ResolveMeteorsHit(world); }, 11.0f);

	Sinistar::SinibombManager sinibombManager(engine);
	engine.RegisterSystem(engine.updateGroup, [sinibombManager = &sinibombManager](Weave::ECS::World& world) { sinibombManager->UpdateSinicrystalHolders(world); });
	engine.RegisterSystem(engine.fixedUpdateGroup, [sinibombManager = &sinibombManager](Weave::ECS::World& world) { sinibombManager->UpdateSinicrystals(world); }, 11.0f);

	Sinistar::CameraManager cameraManager(engine);
	engine.RegisterSystem(engine.updateGroup, [cameraManager = &cameraManager](Weave::ECS::World& world) { cameraManager->UpdateCamera(world); }, 998.0f);

	Sinistar::MinimapManager minimapManager(engine);
	engine.RegisterSystem(engine.uiRenderGroup, [minimapManager = &minimapManager](Weave::ECS::World& world) { minimapManager->DrawMinimap(world); });

	Weave::ECS::EntityID player = Sinistar::CreatePlayer(engine);

	for (int i = 0; i < 300; i++)
	{
		meteorManager.CreateMeteor(engine, { Weave::Random::GenerateRandomInBounds(-100.0f, 100.0f), Weave::Random::GenerateRandomInBounds(-100.0f, 100.0f) });
	}

	Sinistar::PlayerInputs playerInputs(engine);

	engine.Run();

	return 0;
}