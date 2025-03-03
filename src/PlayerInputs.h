#pragma once
#include "Engine/Mathematics.h"
#include "Engine/Engine.h"
#include "Engine/Events.h"
#include "Engine/ECS.h"

class PlayerInputs
{
private:
	Weave::Engine& engine;

	void HandleMouseMoved(Weave::Mathematics::Vector2<float> newMousePos);

public:
	PlayerInputs(Weave::Engine& engine);
};