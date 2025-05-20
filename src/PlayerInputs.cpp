#pragma once
#include "PlayerInputs.h"

void Sinistar::PlayerInputs::HandleMouseMoved(Weave::Mathematics::Vector2<float> mousePos)
{
	targetPosition = mousePos;
}

void Sinistar::PlayerInputs::HandleRightClickStarted()
{
	engine.GetWorld().GetSystemGroup(engine.updateGroup).Subscribe(*this, &PlayerInputs::UpdatePlayerMovement);
}

void Sinistar::PlayerInputs::HandleRightClickEnded()
{
	for (auto [entity, shipMovement, tag] : engine.GetWorld().GetView<ShipMovement, PlayerTag>())
	{
		shipMovement.direction = { 0.0f, 0.0f };
	}

	engine.GetWorld().GetSystemGroup(engine.updateGroup).Unsubscribe(*this, &PlayerInputs::UpdatePlayerMovement);
}

void Sinistar::PlayerInputs::HandleLeftClickStarted()
{
	engine.GetWorld().GetSystemGroup(engine.updateGroup).Subscribe(*this, &PlayerInputs::UpdateBlasterTargetting);

	for (auto [entity, blaster, tag] : engine.GetWorld().GetView<Sinistar::Blasters, PlayerTag>())
	{
		blaster.isFiring = true;
	}
}

void Sinistar::PlayerInputs::HandleLeftClickEnded()
{
	for (auto [entity, blaster, tag] : engine.GetWorld().GetView<Sinistar::Blasters, PlayerTag>())
	{
		blaster.isFiring = false;
	}

	engine.GetWorld().GetSystemGroup(engine.updateGroup).Unsubscribe(*this, &PlayerInputs::UpdateBlasterTargetting);
}

void Sinistar::PlayerInputs::UpdatePlayerMovement(Weave::ECS::World& world)
{
	for (auto [entity, shipMovement, transform, tag] : world.GetView<ShipMovement, Weave::Transform, PlayerTag>())
	{
		shipMovement.direction = (engine.GetRenderer().ScreenToWorldPos(targetPosition) - transform.position).GetUnitVector();
	}
}

void Sinistar::PlayerInputs::UpdatePlayerRotation(Weave::ECS::World& world)
{
	for (auto [entity, transform, tag] : world.GetView<Weave::Transform, PlayerTag>())
	{
		transform.rotation = Weave::Mathematics::RadiansToDegrees(Weave::Mathematics::Vector2<float>::Angle(transform.position, engine.GetRenderer().ScreenToWorldPos(targetPosition)));
	}
}

void Sinistar::PlayerInputs::UpdateBlasterTargetting(Weave::ECS::World& world)
{
	for (auto [entity, transform, blasters, tag] : world.GetView<Weave::Transform, Sinistar::Blasters, PlayerTag>())
	{
		blasters.direction = Weave::Mathematics::AngleToVector(transform.rotation);
	}
}

Sinistar::PlayerInputs::PlayerInputs(Weave::Engine& _engine) : engine(_engine)
{
	engine.GetWorld().GetSystemGroup(engine.updateGroup).Subscribe(*this, &PlayerInputs::UpdatePlayerRotation);

	engine.GetInputHandler().GetMouseMoveEvent().Subscribe(*this, &PlayerInputs::HandleMouseMoved);

	engine.GetInputHandler().GetEvent(sf::Mouse::Button::Right, Weave::Input::InputType::Pressed).Subscribe(*this, &PlayerInputs::HandleRightClickStarted);
	engine.GetInputHandler().GetEvent(sf::Mouse::Button::Right, Weave::Input::InputType::Released).Subscribe(*this, &PlayerInputs::HandleRightClickEnded);

	engine.GetInputHandler().GetEvent(sf::Mouse::Button::Left, Weave::Input::InputType::Pressed).Subscribe(*this, &PlayerInputs::HandleLeftClickStarted);
	engine.GetInputHandler().GetEvent(sf::Mouse::Button::Left, Weave::Input::InputType::Released).Subscribe(*this, &PlayerInputs::HandleLeftClickEnded);
}
