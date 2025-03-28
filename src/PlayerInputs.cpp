#pragma once
#include "PlayerInputs.h"
#include <iostream>
#include <functional>
#include "Engine/Transform.h"
#include "Player.h"

void PlayerInputs::HandleMouseMoved(Weave::Mathematics::Vector2<float> mousePos)
{
	for (auto [entity, transform, tag] : engine.GetWorld().GetView<Weave::Transform, PlayerTag>())
	{
		transform.position = engine.GetRenderer().ScreenToWorldPos(mousePos);
		transform.rotation = Weave::Mathematics::RadiansToDegrees(Weave::Mathematics::Vector2<float>::Angle(transform.position, engine.GetRenderer().ScreenToWorldPos(mousePos)));
	}
}


PlayerInputs::PlayerInputs(Weave::Engine& _engine) : engine(_engine)
{
	engine.GetInputHandler().GetMouseMoveEvent().Subscribe(*this, &PlayerInputs::HandleMouseMoved);
}
