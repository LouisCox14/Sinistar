#pragma once
#include "Camera.h"

Sinistar::CameraManager::CameraManager(Weave::GameEngine& engine)
{
	renderer = &engine.GetRenderer();
}

void Sinistar::CameraManager::UpdateCamera(Weave::ECS::World& world)
{
	for (auto [entity, transform, tag] : world.GetView<Weave::Transform, PlayerTag>())
	{
		renderer->TargetCamera(transform.position + Weave::Mathematics::Vector2<float>(0.0f, 1.25f)); // Adding the extra 1.25f to account for the space taken by the minimap at the top. This is hacky as shit.
	}
}
